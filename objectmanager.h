#pragma once

#include "helperinclude.h"
#include <vector>
#include "sdyshader.h"
#include "sdy.datadefines.glsl"
#include "eulerentity.h"
#include "nodecpu.h"

#include "gpunodedata.h"
#include "gpuparamdata.h"
#include "spopmanagercpu.h"

class GPUNodeData;
class GPUParamData;
class SpopManagerCPU;
class NodeCPU;

// work on this after u have parenting transforms in place
struct SpaceOpNode {
	int parentIndex;
	int distortionType; //should have a NOP (no operation) type (for the separation between branches), move all these glsl type structs to separate header
	int dataIndex;
};

struct GPUSpaceDistortionData {
	int spaceBranchCount;
	vector<SpaceOpNode> spaceBranches;
};

class ObjectManager {
private:
	SDYShader* shader;

	//
	vector<uPtr<NodeCPU>> nodes;

	//
	static dict<SDNodeType, ivec2> byteSizeMap;
	static dict<SDNodeType, string> defaultNameMap;

	//
	GPUNodeData nodeData;
	GPUParamData paramData;
	SpopManagerCPU spopManager;
public:
	friend class GUIManager;
	friend class NodeCPU; friend class ParamCPU; friend class SpopManagerCPU;
	friend class GPUParamData;  friend class GPUNodeData;

	ObjectManager(SDYShader*);

	NodeCPU* root; // always a MIN OP

	static string getDefaultName(SDNodeType);

	NodeCPU* addNode(NodeCPU* parent, SDNodeType type, vec3 pos, vec3 euler, vec3 scale);

	static ivec2 getStructSize(SDNodeType);

	void init();
};