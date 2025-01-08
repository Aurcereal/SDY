#pragma once

#include "helperinclude.h"
#include "sdydatadefines.glsl""
#include <vector>
#include <unordered_map>

class ObjectManager;

struct OpNodeGPU {
	int parentIndex;
	int arrIndex;
	int operationType;
	//float boundingBoxMult = 1.0f; // !
};

struct PrimNodeGPU {
	mat4 invTransform;
	int parentIndex;
	int arrIndex;
	int operationType;
	int distortionIndex;
};

struct GPUNodeData {
private:
	ObjectManager* objectManager;

	dict<SDNodeType, int> nodeCount;

	inline static int primIndexToInterleavedIndex(int i, SDNodeType type) { return i * PRIMCOUNT + type; }

	vector<OpNodeGPU> gpuInternalNodes;
	vector<PrimNodeGPU> gpuLeafNodes;

public:
	inline GPUNodeData(ObjectManager* objectManager) : objectManager(objectManager) {}

	void setOpNode(int i, OpNodeGPU* node);
	void setPrimNode(int i, PrimNodeGPU* node);
	OpNodeGPU* getOpNode(int i);
	PrimNodeGPU* getPrimNode(int i);

	int pushOpNode(int parentIndex, int arrIndex, SDNodeType opType);
	int pushPrimNode(int parentIndex, int arrIndex, SDNodeType type, int distortionIndex); // Created w/Identity Transform
};