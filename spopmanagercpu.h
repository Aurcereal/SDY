#pragma once

#include "helperinclude.h"
#include <unordered_map>

class NodeCPU;
class ObjectManager;

class SpopManagerCPU {
private:
	vector<int> branchStartIndices;
	ObjectManager* objectManager;
	dict<const NodeCPU*, int> spopNodeToDistortionIndex;
	
	vector<vector<const NodeCPU*>> branches;

	int gpuSetBranchElem(int branchIndex, int withinBranchIndex, int paramInd, NodeCPU*);
public:
	int assimilateNewSpopNode(NodeCPU*, int paramInd); // must be SPOP
	int getDistortionIndex(const NodeCPU*);
};