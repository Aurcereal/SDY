struct OpNode {
	int parentIndex;
	int arrIndex;
	int operationType;
	bool visible;
};

struct PrimNode {
	mat4 invTransform;
	int parentIndex;
	int arrIndex;
	int operationType;
	int distortionIndex;
	float boundingBoxMult;
	bool visible;
};

struct SpopNode {
	mat4 invTransform;
	mat4 transform;
	int arrIndex;
	int operationType;
};

layout (std140) uniform OpNodesBlock {
	OpNode nodes[ELEMCOUNT];
} u_OpNodes;
uniform int u_OpNodeCount;

layout (std140) uniform PrimNodesBlock {
	PrimNode nodes[ELEMCOUNT*PRIMCOUNT];
} u_PrimNodes;

layout (std140) uniform SpopNodesBlock {
	SpopNode nodes[ELEMCOUNT];
} u_SpopNodes;
uniform int u_SpopNodeCount;
