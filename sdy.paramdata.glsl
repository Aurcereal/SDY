#include "sdy.datadefines.glsl"

struct Min {
	float a;
};
struct Max {
	float a;
};
struct SMin {
	float smoothness;
};
struct SMax {
	float smoothness;
};

struct Sphere {
	float r;
	// int material index or smth (on SDObject?), have material index file u can also include in cpp, dotted walk over floor spec display mat or smth idk
};
struct Box {
	vec3 dim;
};

struct Twist {
	float intensity;
};
struct Mirror {
	float a;
};
struct Repeat {
	vec3 repDim;
	vec3 repCount;
};
struct Pinch {
	float intensity; // idk lol
};

layout (std140) uniform MinsBlock {
	Min mins[ELEMCOUNT];
} u_Mins;
uniform int u_MinCount;

layout (std140) uniform MaxesBlock {
	Max maxes[ELEMCOUNT];
} u_Maxes;
uniform int u_MaxCount;

layout (std140) uniform SMinsBlock {
	SMin smins[ELEMCOUNT];
} u_SMins;
uniform int u_SMinCount;

layout (std140) uniform SMaxesBlock {
	SMax smaxes[ELEMCOUNT];
} u_SMaxes;
uniform int u_SMaxCount;


layout (std140) uniform SpheresBlock {
	Sphere spheres[ELEMCOUNT];
} u_Spheres;
uniform int u_SphereCount;

layout (std140) uniform BoxesBlock {
	Box boxes[ELEMCOUNT];
} u_Boxes;
uniform int u_BoxCount;


layout (std140) uniform TwistsBlock {
	Twist twists[ELEMCOUNT];
} u_Twists;
uniform int u_TwistCount;

layout (std140) uniform MirrorsBlock {
	Mirror mirrors[ELEMCOUNT];
} u_Mirrors;
uniform int u_MirrorCount;

layout (std140) uniform RepeatsBlock {
	Repeat repeats[ELEMCOUNT];
} u_Repeats;
uniform int u_RepeatCount;

layout (std140) uniform PinchesBlock {
	Pinch pinches[ELEMCOUNT];
} u_Pinches;
uniform int u_PinchCount;
