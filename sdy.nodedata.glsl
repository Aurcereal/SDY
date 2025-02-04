struct SpaceOpNode {
	mat4 invTransform;  // maybe also store transform since we pop in, distort, and pop out usually?
	int parentIndex;
	int arrIndex;
	int operationType;
};

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

layout (std140) uniform OpNodesBlock {
	OpNode nodes[ELEMCOUNT];
} u_OpNodes;
uniform int u_OpNodeCount;

layout (std140) uniform PrimNodesBlock {
	PrimNode nodes[ELEMCOUNT*PRIMCOUNT];
} u_PrimNodes;
