#include "gpunodedata.h"
#include "objectmanager.h"

ivec2 GPUNodeData::opNodeByteSize = ivec2(sizeof(float) * 4, sizeof(vec4));
ivec2 GPUNodeData::primNodeByteSize = ivec2(sizeof(float) * 22, sizeof(vec4) * 6);
ivec2 GPUNodeData::spopNodeByteSize = ivec2(sizeof(float) * 34, sizeof(float) * 36);

void GPUNodeData::setOpNode(int i, OpNodeGPU* node) { gpuOpNodes[i] = *node; objectManager->shader->setOpNodeData(i, node); }
void GPUNodeData::setPrimNode(int i, PrimNodeGPU* node) { gpuPrimNodes[i] = *node; objectManager->shader->setPrimNodeData(i, node); }
void GPUNodeData::setSpopNode(int i, SpopNodeGPU* node) { gpuSpopNodes[i] = *node; objectManager->shader->setSpopNodeData(i, node); }
OpNodeGPU* GPUNodeData::getOpNode(int i) { return &gpuOpNodes[i]; }
PrimNodeGPU* GPUNodeData::getPrimNode(int i) { return &gpuPrimNodes[i]; }
SpopNodeGPU* GPUNodeData::getSpopNode(int i) { return &gpuSpopNodes[i]; }

int GPUNodeData::pushOpNode(int parentIndex, int arrIndex, SDNodeType type) {
	nodeCount[type]++;
	gpuOpNodes.push_back(OpNodeGPU(parentIndex, arrIndex, type));
	objectManager->shader->setOpNodeData(gpuOpNodes.size() - 1, &gpuOpNodes[gpuOpNodes.size()-1]);
	objectManager->shader->uniformOpNodeCount(gpuOpNodes.size());
	return gpuOpNodes.size() - 1;
}

int GPUNodeData::pushPrimNode(int parentIndex, int arrIndex, int distortionIndex, SDNodeType type) {
	int countOfType = nodeCount[type];// objectManager->paramData.getCount(type);
	nodeCount[type]++;
	int placementIndex = primIndexToInterleavedIndex(countOfType, type);
	if (placementIndex >= gpuPrimNodes.size()) gpuPrimNodes.resize(placementIndex + 1);

	gpuPrimNodes[placementIndex] = PrimNodeGPU(parentIndex, arrIndex, distortionIndex, type);
	objectManager->shader->setPrimNodeData(placementIndex, &gpuPrimNodes[placementIndex]);
	return placementIndex;
}

//int GPUNodeData::pushSpopNode(int arrIndex, SDNodeType type) {
//	nodeCount[type]++;
//	gpuSpopNodes.push_back(SpopNodeGPU(arrIndex, type));
//	objectManager->shader->setSpopNodeData(gpuSpopNodes.size() - 1, &gpuSpopNodes[gpuSpopNodes.size() - 1]);
//	objectManager->shader->uniformSpopNodeCount(gpuSpopNodes.size());
//	return gpuSpopNodes.size() - 1;
//}