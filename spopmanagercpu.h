#pragma once

#include "helperinclude.h"

class NodeCPU;

class SpopManagerCPU {
public:
	void assimilateNewSpopNode(NodeCPU*); // must be SPOP
	void getDistortionIndex(NodeCPU*); // must not be SPOP: it's in the middle of a branch
private:
	ObjectManager* objectManager;
	dict<NodeCPU*, int> spopNodeToDistortionIndex;
	//GPUSPopData gpuSpopData; // dont think this is necessary, just use series of push and set and get commands on gpunodedata
};