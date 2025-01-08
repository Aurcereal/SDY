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

struct ObjectAccessor {
	int index;
	SDNodeType type;
};

float dists[MAXSEARCHSIZE];
ObjectAccessor searchStack[MAXSEARCHSIZE];
int searchStackSize;

float debugVal;

vec2 fillSearchStack(vec3 ro, vec3 ird) {
	// first calculate bounding box dim, then try buffering it in
	// youngest child setting the parent thing won't work blindly here, may have to set something here in the shader cuz youngest child may be dead
	// but hey distance array can be shorter if i cant figure smth out

	vec2 searchBounds = vec2(MAXDIST, 0.0);
	searchStackSize = 0;
	for(int i=0; i<u_OpNodeCount; i++) {
		ObjectAccessor accessor;
		accessor.index = i;
		accessor.type = u_OpNodes.nodes[i].operationType;
		searchStack[i] = accessor;
		searchStackSize++;
	}

	for(int i=0; i<u_SphereCount; i++) {
		PrimNode node = u_PrimNodes.nodes[PRIMCOUNT * i + PRIM_SPHERE];
		Sphere s = u_Spheres.spheres[node.arrIndex];

		vec3 lro = ro + vec3(vec4(node.invTransform[3].xyz, 0.0) * node.invTransform );
		vec3 boxDim = vec3(2.0*s.r);

		vec2 ts = rayBoxIntersect(lro, ird, boxDim);
		if(ts.x <= ts.y && ts.y >= 0.0) {
			ObjectAccessor accessor;
			accessor.index = PRIMCOUNT * i + PRIM_SPHERE;
			accessor.type = PRIM_SPHERE;
			searchStack[searchStackSize] = accessor;
			searchStackSize++;

			searchBounds.x = min(ts.x, searchBounds.x);
			searchBounds.y = max(ts.y, searchBounds.y);
		}
	}

	for(int i=0; i<u_BoxCount; i++) {
		PrimNode node = u_PrimNodes.nodes[PRIMCOUNT * i + PRIM_BOX];
		Box b = u_Boxes.boxes[node.arrIndex];

		vec3 lro = ro + vec3(vec4(node.invTransform[3].xyz, 0.0) * node.invTransform );
		vec3 boxDim = b.dim*1.732;

		vec2 ts = rayBoxIntersect(lro, ird, boxDim);
		if(ts.x <= ts.y && ts.y >= 0.0) {
			ObjectAccessor accessor;
			accessor.index = PRIMCOUNT * i + PRIM_BOX;
			accessor.type = PRIM_BOX;
			searchStack[searchStackSize] = accessor;
			searchStackSize++;

			searchBounds.x = min(ts.x, searchBounds.x);
			searchBounds.y = max(ts.y, searchBounds.y);
		}
	}

	return searchBounds;
}

float sdOperationStack(vec3 p) {
	for(int i=0; i<searchStackSize; i++) {
		// use bit slicing operations where a bit can correspond to min or max and another bit can correpsond to smooth or not
		//if(u_Operations.nodes[searchStack[i]].operationType == OP_MIN || u_Operations.nodes[searchStack[i]].operationType == OP_SMIN) {
		//	dists[i] = MAXDIST;
		//} else {
		//	dists[i] = -MAXDIST;
		//}
		dists[i] = MAXDIST;
	}

	// for smin and stuff can have like a bounding box modifier like i guess... float boundingBoxmMultSTack[16] ?? .. or a similar struct. bit early to tell
	for(int i=searchStackSize-1; i>=u_OpNodeCount; i--) {
		ObjectAccessor accessor = searchStack[i];
		PrimNode node = u_PrimNodes.nodes[accessor.index];
		int parentIndex = node.parentIndex;
		vec3 lp = (node.invTransform * vec4(p, 1.0)).xyz;
		switch(node.operationType) {
			case PRIM_SPHERE:
				Sphere sphere = u_Spheres.spheres[node.arrIndex];
				dists[i] = length(lp) - sphere.r;
				break;
			case PRIM_BOX:
				Box box = u_Boxes.boxes[node.arrIndex];
				dists[i] = sdBox(lp, box.dim);
				break;
		}

		OpNode parentNode = u_OpNodes.nodes[parentIndex];
		switch(parentNode.operationType) {
			case OP_MIN:
				dists[parentIndex] = min(dists[parentIndex], dists[i]);
				if(i == 3) return dists[i];
				break;
			case OP_SMIN:
				dists[parentIndex] = smin(dists[parentIndex], dists[i], 0.5);
				break;
		}
	}

	for(int i=u_OpNodeCount-1; i>=0; i--) {
		// later i dont actually have to add the operations to the stack, need to allocate space for them on dist array is all
		ObjectAccessor accessor = searchStack[i];
		OpNode n = u_OpNodes.nodes[accessor.index];

		if(n.parentIndex != -1) {
			OpNode parentNode = u_OpNodes.nodes[n.parentIndex];
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
	return sdOperationStack(p);
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
	searchBounds = vec2(0.0, MAXDIST);

	// The search bounds optimization doesn't seem to help much since we're already hitting the object soon and terminating
	float dist = trace(ro, rd, searchBounds);

	return shade(ro, rd, dist, dist * dot(rd, u_CamFo), normal(ro+rd*dist));
}

///

void main() {
	debugVal = 0.0;
	vec2 p = (gl_FragCoord.xy/(1.0*u_ScreenDimensions))*2.0-1.0;
	vec3 col = render(p);

	#ifdef VISUALIZEBOUNDINGBOX
	FragColor = vec4(searchStackSize/5.0, col.y, col.z, 1.0);
	#else
	FragColor = vec4(col, 1.0);
	#endif
}
