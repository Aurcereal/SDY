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

//#define VISUALIZEBOUNDINGBOX

#define PI 3.141592
#define TAU (2.0*PI)

#include "raymarchdefines.glsl"

#include "primitives.glsl"

#include "common.glsl"

// If this doesn't expand we can just use an int (no vec4 alignment)
struct ObjectAccessor {
	int index;
};

// Later can recompile to change size of this arr (change the #define); can start small and when cross certain thresholds gets larger like ArrayList
float dists[MAXSEARCHSIZE];
ObjectAccessor searchStack[MAXSEARCHSIZE];
int searchStackSize;

bool opCanExist[16];
bool what[2];

vec2 fillSearchStack(vec3 ro, vec3 rd) {
	vec2 searchBounds = vec2(MAXDIST, 0.0);
	searchStackSize = 0;

	for(int i=0; i<u_OpNodeCount; i++) {
		switch(u_OpNodes.nodes[i].operationType) {
			case OP_MAX:
			case OP_SMAX:
			opCanExist[i] = true;
			break;

			case OP_MIN:
			case OP_SMIN:
			opCanExist[i] = false;
			break;
		}
	}

	for(int i=0; i<u_SphereCount; i++) {
		PrimNode node = u_PrimNodes.nodes[PRIMCOUNT * i + PRIM_SPHERE];
		if(!node.visible) continue;
		Sphere s = u_Spheres.spheres[node.arrIndex];

		vec3 lro = (node.invTransform * vec4(ro,1.)).xyz;
		vec3 lrd = (node.invTransform * vec4(rd, 0.)).xyz;
		vec3 boxDim = vec3(2.0*s.r) * node.boundingBoxMult;

		vec2 ts = rayBoxIntersect(lro, 1./lrd, boxDim);
		bool hit = ts.x <= ts.y && ts.y >= 0.0;
		if(hit) {
			ObjectAccessor accessor;
			accessor.index = PRIMCOUNT * i + PRIM_SPHERE;
			searchStack[searchStackSize] = accessor;
			searchStackSize++;

			searchBounds.x = min(ts.x, searchBounds.x);
			searchBounds.y = max(ts.y, searchBounds.y);

		}

		switch(u_OpNodes.nodes[node.parentIndex].operationType) {
			case OP_MAX:
			case OP_SMAX:
			opCanExist[node.parentIndex] = opCanExist[node.parentIndex] && hit;
			break;

			case OP_MIN:
			case OP_SMIN:
			opCanExist[node.parentIndex] = opCanExist[node.parentIndex] || hit;
			break;
		}
	}

	for(int i=0; i<u_BoxCount; i++) {
		PrimNode node = u_PrimNodes.nodes[PRIMCOUNT * i + PRIM_BOX];
		if(!node.visible) continue;
		Box b = u_Boxes.boxes[node.arrIndex];

		vec3 lro = (node.invTransform * vec4(ro, 1.)).xyz;
		vec3 lrd = (node.invTransform * vec4(rd, 0.)).xyz;
		vec3 boxDim = b.dim * node.boundingBoxMult;

		vec2 ts = rayBoxIntersect(lro, 1./lrd, boxDim);
		bool hit = ts.x <= ts.y && ts.y >= 0.0;
		if(hit) {
			ObjectAccessor accessor;
			accessor.index = PRIMCOUNT * i + PRIM_BOX;
			searchStack[searchStackSize] = accessor;
			searchStackSize++;

			searchBounds.x = min(ts.x, searchBounds.x);
			searchBounds.y = max(ts.y, searchBounds.y);
		}

		switch(u_OpNodes.nodes[node.parentIndex].operationType) {
			case OP_MAX:
			case OP_SMAX:
			opCanExist[node.parentIndex] = opCanExist[node.parentIndex] && hit;
			break;

			case OP_MIN:
			case OP_SMIN:
			opCanExist[node.parentIndex] = opCanExist[node.parentIndex] || hit;
			break;
		}
	}

	for(int i=u_OpNodeCount-1; i>=1; i--) {
		int parentIndex = u_OpNodes.nodes[i].parentIndex;
		if(parentIndex == -1) continue;
		bool exist = opCanExist[i];
		switch(u_OpNodes.nodes[parentIndex].operationType) {
			case OP_MAX:
			case OP_SMAX:
			opCanExist[parentIndex] = opCanExist[parentIndex] && exist;
			break;

			case OP_MIN:
			case OP_SMIN:
			opCanExist[parentIndex] = opCanExist[parentIndex] || exist;
			break;
		}
	}

	return searchBounds;
}

float sdOperationStack(vec3 p) {
	for(int i=0; i<u_OpNodeCount; i++) {
		// later mabe use bit slicing operations where a bit can correspond to min or max and another bit can correpsond to smooth or not
		dists[i] = u_OpNodes.nodes[i].operationType <= -3 ? -NULLMAXDIST : NULLMAXDIST;
	}

	for(int i=searchStackSize-1; i>=0; i--) {
		// If prim is on the stack it's def visible
		ObjectAccessor accessor = searchStack[i];
		PrimNode node = u_PrimNodes.nodes[accessor.index];
		int parentIndex = node.parentIndex;

		float sd;

		vec3 lp = (node.invTransform * vec4(p, 1.0)).xyz;

		switch(node.operationType) {
			case PRIM_SPHERE:
				Sphere sphere = u_Spheres.spheres[node.arrIndex];
				sd = length(lp) - sphere.r;
				break;
			case PRIM_BOX:
				Box box = u_Boxes.boxes[node.arrIndex];
				sd = sdBox(lp, box.dim);
				break;
		}

		OpNode parentNode = u_OpNodes.nodes[parentIndex];
		switch(parentNode.operationType) {
			case OP_MIN:
				dists[parentIndex] = min(dists[parentIndex], sd);
				break;
			case OP_SMIN:
				dists[parentIndex] = smin(dists[parentIndex], sd, u_SMins.smins[parentNode.arrIndex].smoothness);
				break;
			case OP_MAX:
				dists[parentIndex] = max(dists[parentIndex], sd);
				break;
			case OP_SMAX:
				dists[parentIndex] = smax(dists[parentIndex], sd, u_SMaxes.smaxes[parentNode.arrIndex].smoothness);
				break;
		}
	}

	for(int i=u_OpNodeCount-1; i>=0; i--) {
		OpNode n = u_OpNodes.nodes[i];
		bool exist = opCanExist[i] && n.visible;

		if(exist && n.parentIndex != -1 && abs(dists[i]) < NULLMAXDIST) { // Possible floating point errs?
			OpNode parentNode = u_OpNodes.nodes[n.parentIndex];
			switch(parentNode.operationType) {
				case OP_MIN:
					dists[n.parentIndex] = min(dists[n.parentIndex], dists[i]);
					break;
				case OP_SMIN:
					dists[n.parentIndex] = smin(dists[n.parentIndex], dists[i], u_SMins.smins[parentNode.arrIndex].smoothness);
					break;
				case OP_MAX:
					dists[n.parentIndex] = max(dists[n.parentIndex], dists[i]);
					break;
				case OP_SMAX:
					dists[n.parentIndex] = smax(dists[n.parentIndex], dists[i], u_SMaxes.smaxes[parentNode.arrIndex].smoothness);
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
	vec2 searchBounds = fillSearchStack(ro, rd);
	searchBounds = vec2(0.0, MAXDIST);

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
