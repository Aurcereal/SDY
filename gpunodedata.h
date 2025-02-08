#pragma once

#include "helperinclude.h"
#include "sdy.datadefines.glsl"
#include <vector>
#include <unordered_map>

class ObjectManager;

struct OpNodeGPU {
	int parentIndex;
	int arrIndex;
	int operationType;
	bool visible = true;
	inline OpNodeGPU(int parentIndex, int arrIndex, SDNodeType type) : parentIndex(parentIndex), arrIndex(arrIndex), operationType(type) {}
};

struct PrimNodeGPU {
	mat4 invTransform;
	int parentIndex;
	int arrIndex;
	int operationType;
	int distortionIndex;
	float boundingBoxMult = 1.0f;
	bool visible = true; // We know this is 32-bit in shader, but be careful in C++ it's like platform dependent or some shit, can just use an int honestly (check this msg)
	inline PrimNodeGPU(int parentIndex, int arrIndex, int distortionIndex, SDNodeType type) : 
		parentIndex(parentIndex), arrIndex(arrIndex), operationType(type), distortionIndex(distortionIndex), invTransform(1.0f), boundingBoxMult(1.0f), visible(true) {}
	inline PrimNodeGPU() : PrimNodeGPU(-1, -1, -1, 0) {} // doesn't compile if I don't have explicit default constructor idk y
};

struct SpopNodeGPU {
	mat4 invWorldTransform;
	mat4 worldTransform;
	int arrIndex;
	int operationType;
	inline SpopNodeGPU(int arrIndex, SDNodeType type) : 
		arrIndex(arrIndex), operationType(type), invWorldTransform(1.0f), worldTransform(1.0f) {}
};

struct GPUNodeData {
private:
	ObjectManager* objectManager;

	dict<SDNodeType, int> nodeCount;

	inline static int primIndexToInterleavedIndex(int i, SDNodeType type) { return i * PRIMCOUNT + type; }

	vector<OpNodeGPU> gpuOpNodes;
	vector<PrimNodeGPU> gpuPrimNodes;
	vector<SpopNodeGPU> gpuSpopNodes;

public:
	inline GPUNodeData(ObjectManager* objectManager) : objectManager(objectManager) {}

	void setOpNode(int i, OpNodeGPU* node);
	void setPrimNode(int i, PrimNodeGPU* node);
	void setSpopNode(int i, SpopNodeGPU* node);
	OpNodeGPU* getOpNode(int i);
	PrimNodeGPU* getPrimNode(int i);
	SpopNodeGPU* getSpopNode(int i);

	int pushOpNode(int parentIndex, int arrIndex, SDNodeType opType);
	int pushPrimNode(int parentIndex, int arrIndex, int distortionIndex, SDNodeType type); // Created w/Identity Transform
	int pushSpopNode(int arrIndex, SDNodeType type);

	static ivec2 opNodeByteSize;
	static ivec2 primNodeByteSize;
	static ivec2 spopNodeByteSize;
};