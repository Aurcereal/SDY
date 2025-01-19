#include "sdydatadefines.glsl"

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

struct OpNode {
	int parentIndex;
	int arrIndex;
	int operationType;
};

struct PrimNode {
	mat4 invTransform;
	int parentIndex;
	int arrIndex;
	int operationType;
	int distortionIndex;
	float boundingBoxMult;
};

layout (std140) uniform OpNodesBlock {
	OpNode nodes[ELEMCOUNT];
} u_OpNodes;
uniform int u_OpNodeCount;

layout (std140) uniform PrimNodesBlock {
	PrimNode nodes[ELEMCOUNT*PRIMCOUNT];
} u_PrimNodes;