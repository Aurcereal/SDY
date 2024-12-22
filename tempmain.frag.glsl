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

#define MAXDIST 50.0
#define MAXSTEPS 100
#define SDEPS 0.0005
#define NORMEPS 0.0005

struct Sphere {
	//vec3 pos;
	//float r;
	vec4 data; // test out alignment and stuff by i nterchanging vec3 and float
};

layout (std140) uniform SpheresBlock {
	vec4 spheres[256];
} u_Spheres;
uniform int u_SphereCount;

#include "testsdfile.glsl"

float sdf(vec3 p) {
	float d = length(p-vec3(0.0, 0.0, 5.0)) - 1.0;
	for(int i=0; i<u_SphereCount; i++) {
		d = min(d, sdSphere(p-u_Spheres.spheres[i].xyz, u_Spheres.spheres[i].w));//min(d, length(p-u_Spheres.spheres[i].xyz) - u_Spheres.spheres[i].w);
	}
	return d;
}

float trace(vec3 ro, vec3 rd) {
	float sd, d = 0.0;

	for(int i=0; i<MAXSTEPS; i++) {
		sd = sdf(ro+rd*d);
		d += sd;

		if(abs(sd) <= SDEPS || d >= MAXDIST) {
			return d;
		}
	}

	return MAXDIST;
}

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

	float dist = trace(ro, rd);

	return shade(ro, rd, dist, dist * dot(rd, u_CamFo), normal(ro+rd*dist));
}

///

void main() {
	vec2 p = (gl_FragCoord.xy/(1.0*u_ScreenDimensions))*2.0-1.0;
	vec3 col = render(p);

	FragColor = vec4(col, 1.0);
}
