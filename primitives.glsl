#include "sdydatadefines.glsl"

struct Sphere {
	mat4 invTransform;
	float r;
	// int material index or smth, have material index file u can also include in cpp, dotted walk over floor spec display mat or smth idk
};

layout (std140) uniform SpheresBlock {
	Sphere spheres[ELEMCOUNT];
} u_Spheres;
//uniform int u_SphereCount;

// Node = (operationType = [0...operTypeCount], childIndex1 >= 0, childIndex2 >= 0) grab children from scene graph
// LeafNode = (operationType = [-1...-primTypeCount], childIndex1 >= 0, childIndex2 = -1) grab children from corresponding primitive block based on operationtype

struct Node { // but glsl isn't good with ints?
	int parentIndex;
	int operationType;
	int objectIndex;
};

layout (std140) uniform OperationsBlock {
	Node nodes[ELEMCOUNT]; // 16 each
} u_Operations;
uniform int u_OperationCount;