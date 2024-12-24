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
#define TAU 2.0*PI

#include "raymarchdefines.glsl"

#include "primitives.glsl"

Sphere sphereDrawStack[12]; // this can be smaller when we do bbx culling
int sphereDrawStackSize = 0;

void fillDrawStacks() {
	// for smin and stuff can have like a bounding box modifier like i guess... float boundingBoxmMultSTack[16] ?? .. or a similar struct. bit early to tell
	int searchStack[16];

	searchStack[0] = 0;
	int stackSize = 1;

	while(stackSize > 0) {
		Node n = u_Operations.nodes[searchStack[stackSize-1]];
		stackSize--;

		if(n.operationType >= 0) {
			// calculating all of them MAY be more efficient than branching?? whatever
			// this is where u'd do the bounding box stuff
			sphereDrawStack[sphereDrawStackSize] = u_Spheres.spheres[n.childIndex1];
			sphereDrawStackSize++;
		} else {
			searchStack[stackSize] = n.childIndex1;
			searchStack[stackSize+1] = n.childIndex2;
			stackSize += 2;
		}
	}
}

float sdf(vec3 p) {
	float d = length(p-vec3(0.0, 0.0, 5.0)) - 1.0;

	for(int i=0; i<sphereDrawStackSize; i++) {
		Sphere sphere = sphereDrawStack[i];
		d = min(d, length(p - sphere.pos) - sphere.r);
	}

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
		return vec3(0.0);
	}

	return norm*0.5+0.5;
}

//
vec3 render(vec2 p) {
	float aspect = (1.0*u_ScreenDimensions.x)/u_ScreenDimensions.y;

	vec3 ro = u_CamPos;
	vec3 rd = mat3(u_CamRi, u_CamUp, u_CamFo) * normalize(vec3(p*tan(u_fovY*0.5)*vec2(aspect, 1.0), 1.0));

	fillDrawStacks();
	float dist = trace(ro, rd);

	return shade(ro, rd, dist, dist * dot(rd, u_CamFo), normal(ro+rd*dist));
}

///

void main() {
	vec2 p = (gl_FragCoord.xy/(1.0*u_ScreenDimensions))*2.0-1.0;
	vec3 col = render(p);

	FragColor = vec4(col, 1.0);
}
