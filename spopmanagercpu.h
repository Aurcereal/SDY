#pragma once

class NodeCPU;

class SpopManagerCPU {
public:
	void assimilateNewSpopNode(NodeCPU*); // must be SPOP
	void getDistortionIndex(NodeCPU*); // must not be SPOP: it's in the middle of a branch
private:
	GPUSPopData gpuSpopData;
};