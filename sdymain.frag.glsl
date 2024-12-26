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

#define PI 3.141592
#define TAU (2.0*PI)

#include "raymarchdefines.glsl"

#include "primitives.glsl"

#include "common.glsl"

// TODO: this is for each frag or whatever, somehow it shouldn't be ELEMCOUNT.  technically doesn't have to be since the objects that are intersecting should p much always be less
// than ELEMCOUNT... we can make that assumption OK if not that'd be fucked.  anyways yeah remember that bounding box jumping dead stuff earlier, can use some shit to do mapping
// to have a shorter array where we're not storing dists of dead nodes
float dists[DISTELEMCOUNT];

float sdOperationStack(vec3 p) {
	for(int i=0; i<u_OperationCount; i++) {
		// use bit slicing operations where a bit can correspond to min or max and another bit can correpsond to smooth or not
		if(u_Operations.nodes[i].operationType == OP_MIN || u_Operations.nodes[i].operationType == OP_SMIN) {
			dists[i] = MAXDIST;
		} else {
			dists[i] = -MAXDIST;
		}
	}

	// why does this second for loop cost so much performance but not the first one?  even 10 vs 50 objects.. is it the copying of obj structs? switch statements?? WHAT. gotta debug later
	// for smin and stuff can have like a bounding box modifier like i guess... float boundingBoxmMultSTack[16] ?? .. or a similar struct. bit early to tell
	// for speed optimization maybe when i delete certain nodes, can 'delete' them from the array by having like an array of [n1, n2, nDead, nDead, n5] and n5 has a backoffset of -3
	// so we know to jump -3 to get to n2, but has to be per-frag array
	for(int i=u_OperationCount-1; i>=0; i--) {
		Node n = u_Operations.nodes[i];

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

	float dist = trace(ro, rd);

	return shade(ro, rd, dist, dist * dot(rd, u_CamFo), normal(ro+rd*dist));
}

///

void main() {
	vec2 p = (gl_FragCoord.xy/(1.0*u_ScreenDimensions))*2.0-1.0;
	vec3 col = render(p);

	FragColor = vec4(col, 1.0);
}
