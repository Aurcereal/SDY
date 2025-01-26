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
	bool visible = true;
};

struct PrimNodeGPU {
	mat4 invTransform;
	int parentIndex;
	int arrIndex;
	int operationType;
	int distortionIndex;
	float boundingBoxMult = 1.0f;
	bool visible = true; // We know this is 32-bit in shader, but be careful in C++ it's like platform dependent or some shit, can just use an int honestly
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