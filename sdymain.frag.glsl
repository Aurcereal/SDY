#version 330 core

uniform float u_Time;

in vec4 fs_Pos;

out vec4 FragColor;

///

uniform vec3 u_CamPos;

uniform vec3 u_CamRi;
uniform vec3 u_CamUp;
uniform vec3 u_CamFo;

uniform float u_fovY;

uniform ivec2 u_ScreenDimensions;

///

#define VISUALIZEBOUNDINGBOX

#define PI 3.141592
#define TAU (2.0*PI)

#include "raymarchdefines.glsl"

#include "primitives.glsl"

#include "common.glsl"

// TODO: this is for each frag or whatever, somehow it shouldn't be ELEMCOUNT.  technically doesn't have to be since the objects that are intersecting should p much always be less
// than ELEMCOUNT... we can make that assumption OK if not that'd be fucked.  anyways yeah remember that bounding box jumping dead stuff earlier, can use some shit to do mapping
// to have a shorter array where we're not storing dists of dead nodes
float dists[DISTELEMCOUNT];

// Rule: All non-prim operations must have indices lower than all prim operations
// Another relevant rule: All non-prim operations will be added to the search stack always for now.
// These imply that for an arb non-prim operation 'o', index of o in searchStack equals index of o in u_Operations.nodes
// I can't maintain that first rule for long tho cuz if I wanna add a MIN operation, don't wanna shift everything.  I think I'll have a separate
// buffer for operations and leaf nodes later. (1 buffer for internal nodes, 1 buffer for leaf nodes)
int searchStack[DISTELEMCOUNT];
int searchStackSize;

vec2 fillSearchStack(vec3 ro, vec3 ird) {
	// first calculate bounding box dim, then try buffering it in
	// youngest child setting the parent thing won't work blindly here, may have to set something here in the shader cuz youngest child may be dead
	// but hey distance array can be shorter if i cant figure smth out

	vec2 searchBounds = vec2(MAXDIST, 0.0);
	searchStackSize = 0;
	for(int i=0; i<u_OperationCount; i++) {
		Node n = u_Operations.nodes[i];

		if(n.operationType < 0) {
			searchStack[searchStackSize] = i;
			searchStackSize++;
			continue;
		} else {
			vec3 lro = ro;
			vec3 boxDim;
			switch(n.operationType) {
				case PRIM_SPHERE:
					lro = ro + u_Spheres.spheres[n.objectIndex].invTransform[3].xyz;
					boxDim = vec3(2.0*u_Spheres.spheres[n.objectIndex].r);
					break;
				case PRIM_BOX:
					lro = ro + u_Boxes.boxes[n.objectIndex].invTransform[3].xyz;
					boxDim = u_Boxes.boxes[n.objectIndex].dim*1.732;
					break;
			}

			vec2 ts = rayBoxIntersect(lro, ird, boxDim);
			if(ts.x <= ts.y && ts.y >= 0.0) {
				// Later make it return vec2 or smth so we march from start to back
				searchStack[searchStackSize] = i;
				searchStackSize++;

				searchBounds.x = min(ts.x, searchBounds.x);
				searchBounds.y = max(ts.y, searchBounds.y);
			}
		}
	}

	return searchBounds;
}

float sdOperationStack(vec3 p) {
	for(int i=0; i<searchStackSize; i++) {
		// use bit slicing operations where a bit can correspond to min or max and another bit can correpsond to smooth or not
		if(u_Operations.nodes[searchStack[i]].operationType == OP_MIN || u_Operations.nodes[searchStack[i]].operationType == OP_SMIN) {
			dists[i] = MAXDIST;
		} else {
			dists[i] = -MAXDIST;
		}
	}

	// why does this second for loop cost so much performance but not the first one?  even 10 vs 50 objects.. is it the copying of obj structs? switch statements?? WHAT. gotta debug later
	// for smin and stuff can have like a bounding box modifier like i guess... float boundingBoxmMultSTack[16] ?? .. or a similar struct. bit early to tell
	// for speed optimization maybe when i delete certain nodes, can 'delete' them from the array by having like an array of [n1, n2, nDead, nDead, n5] and n5 has a backoffset of -3
	// so we know to jump -3 to get to n2, but has to be per-frag array
	for(int i=searchStackSize-1; i>=0; i--) {
		Node n = u_Operations.nodes[searchStack[i]];

		if(n.objectIndex != -1) {
			// Leaves should only have objects, operationType should be a PRIM
			vec3 lp;
			switch(n.operationType) {
				case PRIM_SPHERE:
					Sphere sphere = u_Spheres.spheres[n.objectIndex];
					lp = (sphere.invTransform * vec4(p, 1.0)).xyz;
					dists[i] = length(lp) - sphere.r;
					break;
				case PRIM_BOX:
					Box box = u_Boxes.boxes[n.objectIndex];
					lp = (box.invTransform * vec4(p, 1.0)).xyz;
					dists[i] = sdBox(lp, box.dim);
					break;
			}
		}

		if(n.parentIndex != -1) {
			// Internal nodes should only have operations, operationType should be an OP
			Node parentNode = u_Operations.nodes[n.parentIndex];
			switch(parentNode.operationType) {
				case OP_MIN:
					// dists[n.parentIndex] uses rule on line 39
					dists[n.parentIndex] = min(dists[n.parentIndex], dists[i]);
					break;
			}
		}
	}

	return dists[0];
}

float sdf(vec3 p) {
	float d = length(p-vec3(0.0, 0.0, 5.0)) - 1.0;

	d = min(d, sdOperationStack(p));

	return d;
}

#include "raymarching.glsl"

vec3 normal(vec3 p) {
	vec2 ep = vec2(NORMEPS, 0.0);
	return normalize(vec3(
		sdf(p + ep.xyy) - sdf(p - ep.xyy),
		sdf(p + ep.yxy) - sdf(p - ep.yxy),
		sdf(p + ep.yyx) - sdf(p - ep.yyx)
					));
}

vec3 shade(vec3 camPos, vec3 rd, float dist, float distAlongCam, vec3 norm) {
	if(dist >= MAXDIST) {
		return 1.5 * (acos(rd.y)/PI) * vec3(134.0, 206.0, 203.0)/255.0;
	}

	return norm*0.5+0.5;
}

//
vec3 render(vec2 p) {
	float aspect = (1.0*u_ScreenDimensions.x)/u_ScreenDimensions.y;

	vec3 ro = u_CamPos;
	vec3 rd = mat3(u_CamRi, u_CamUp, u_CamFo) * normalize(vec3(p*tan(u_fovY*0.5)*vec2(aspect, 1.0), 1.0));
	vec2 searchBounds = fillSearchStack(ro, 1./rd);

	// The search bounds optimization doesn't seem to help much since we're already hitting the object soon and terminating
	float dist = trace(ro, rd, searchBounds);

	return shade(ro, rd, dist, dist * dot(rd, u_CamFo), normal(ro+rd*dist));
}

///

void main() {
	vec2 p = (gl_FragCoord.xy/(1.0*u_ScreenDimensions))*2.0-1.0;
	vec3 col = render(p);

	#ifdef VISUALIZEBOUNDINGBOX
	FragColor = vec4(searchStackSize/5.0, col.y, col.z, 1.0);
	#else
	FragColor = vec4(col, 1.0);
	#endif
}
