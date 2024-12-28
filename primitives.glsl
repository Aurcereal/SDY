#include "sdydatadefines.glsl"

struct Sphere {
	mat4 invTransform;
	int parentIndex;
	float r;
	// int material index or smth (on SDObject?), have material index file u can also include in cpp, dotted walk over floor spec display mat or smth idk
};

struct Box {
	mat4 invTransform;
	int parentIndex;
	vec3 dim;
};

layout (std140) uniform SpheresBlock {
	Sphere spheres[ELEMCOUNT];
} u_Spheres;
uniform int u_SphereCount;

layout (std140) uniform BoxesBlock {
	Box boxes[ELEMCOUNT];
} u_Boxes;
uniform int u_BoxCount;

struct Node { // but glsl isn't good with ints?
	int parentIndex;
	int operationType;
};

layout (std140) uniform OperationsBlock {
	Node nodes[ELEMCOUNT]; // 16 each
} u_Operations;
uniform int u_OperationCount;