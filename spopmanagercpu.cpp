#include "spopmanagercpu.h"
#include "nodecpu.h"
#include "objectmanager.h"

int SpopManagerCPU::assimilateNewSpopNode(NodeCPU* node, int paramInd) {
	assert(NodeCPU::isSpopNode(node->type));
	assert(node->parent != nullptr); // Spop node can't be root

	// Find first Spop Ancestor
	const NodeCPU* spopAncestor = node->parent;
	while (spopAncestor != nullptr && !NodeCPU::isSpopNode(spopAncestor->type)) {
		spopAncestor = spopAncestor->parent;
	}

	if (spopAncestor != nullptr) {
		if (node->parent != spopAncestor || node->parent->children.size() > 1) {
			// Branch off, copy branch

			branches.push_back(vector<const NodeCPU*>());
			vector<const NodeCPU*>& copyFromBranch = branches[getDistortionIndex(spopAncestor)];
			vector<const NodeCPU*>& copyToBranch = branches[branches.size() - 1];
			for (auto node : copyFromBranch) {
				copyToBranch.push_back(node);
				if (node == spopAncestor) break;
			}
			copyToBranch.push_back(node);
			spopNodeToDistortionIndex[node] = branches.size() - 1;

			return gpuSetBranchElem(branches.size() - 1, branches[branches.size() - 1].size() - 1, paramInd, node);
		}
		else {
			// Continue branch
			int branchIndex = getDistortionIndex(spopAncestor);
			branches[branchIndex].push_back(node);
			spopNodeToDistortionIndex[node] = branchIndex;
			
			return gpuSetBranchElem(branchIndex, branches[branchIndex].size() - 1, paramInd, node);
		}
	}
	else {
		// Empty branch
		branches.push_back({ node });
		spopNodeToDistortionIndex[node] = branches.size() - 1;

		return gpuSetBranchElem(branches.size() - 1, 0, paramInd, node);
	}
}

int SpopManagerCPU::getDistortionIndex(const NodeCPU* node) {
	assert(spopNodeToDistortionIndex.count(node) != 0);
	return spopNodeToDistortionIndex[node];
}

int SpopManagerCPU::gpuSetBranchElem(int branchIndex, int withinBranchIndex, int paramInd, NodeCPU* node) {
	//int arrIndexNode = objectManager->nodeData.spopNodeArrayAllocateUpToSize(branchIndex, withinBranchIndex);
	//int arrIndexNop = objectManager->nodeData.spopNodeArrayAllocateUpToSize(branchIndex, withinBranchIndex+1);
	//auto spopNode = SpopNodeGPU(paramInd, node->type);
	//auto spopNop = SpopNodeGPU(-1, SPOP_NOP); // We must append SPOPs to the end to know when to stop
	//objectManager->nodeData.setSpopNode(arrIndexNode, &spopNode);
	//objectManager->nodeData.setSpopNode(arrIndexNop, &spopNop);

	return -1; // TODO: implement
}