#include "gpunodedata.h"
#include "objectmanager.h"

void GPUNodeData::setOpNode(int i, OpNodeGPU* node) { gpuInternalNodes[i] = *node; objectManager->shader->setOpNodeData(i, node); }
void GPUNodeData::setPrimNode(int i, PrimNodeGPU* node) { gpuLeafNodes[i] = *node; objectManager->shader->setPrimNodeData(i, node); }
OpNodeGPU* GPUNodeData::getOpNode(int i) { return &gpuInternalNodes[i]; }
PrimNodeGPU* GPUNodeData::getPrimNode(int i) { return &gpuLeafNodes[i]; }

int GPUNodeData::pushOpNode(int parentIndex, int arrIndex, SDNodeType type) {
	nodeCount[type]++;
	gpuInternalNodes.push_back(OpNodeGPU());
	OpNodeGPU& node = gpuInternalNodes[gpuInternalNodes.size() - 1];
	node.parentIndex = parentIndex;
	node.arrIndex = arrIndex;
	node.operationType = type;
	objectManager->shader->setOpNodeData(gpuInternalNodes.size() - 1, &node);
	objectManager->shader->uniformOpNodeCount(gpuInternalNodes.size());
	return gpuInternalNodes.size() - 1;
}

int GPUNodeData::pushPrimNode(int parentIndex, int arrIndex, SDNodeType type, int distortionIndex) {
	int countOfType = nodeCount[type];// objectManager->paramData.getCount(type);
	nodeCount[type]++;
	int placementIndex = primIndexToInterleavedIndex(countOfType, type);
	if (placementIndex >= gpuLeafNodes.size()) gpuLeafNodes.resize(placementIndex + 1);

	gpuLeafNodes[placementIndex] = PrimNodeGPU();
	PrimNodeGPU& node = gpuLeafNodes[placementIndex];
	node.parentIndex = parentIndex;
	node.arrIndex = arrIndex;
	node.operationType = type;
	node.distortionIndex = distortionIndex;
	node.invTransform = mat4(1.0f);
	node.boundingBoxMult = 1.0f;

	objectManager->shader->setPrimNodeData(placementIndex, &node);
	return placementIndex;
}