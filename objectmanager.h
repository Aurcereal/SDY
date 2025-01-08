#pragma once

#include "helperinclude.h"
#include <vector>
#include "sdyshader.h"
#include "sdydatadefines.glsl"
#include "eulerentity.h"
#include "nodecpu.h"

#include "gpunodedata.h"
#include "gpuparamdata.h"

class GPUNodeData;
class GPUParamData;
class NodeCPU;

// work on this after u have parenting transforms in place
struct SpaceOpNode {
	int parentIndex;
	int distortionType; //should have a NOP (no operation) type (for the separation between branches), move all these glsl type structs to separate header
	int dataIndex;
};

//struct NodeAccessor {
//	SDNodeType type;
//	int index;
//	inline NodeAccessor(SDNodeType type, int index) : type(type), index(index) {}
//	inline bool isLeaf() { return type >= 0; }
//	inline bool isNull() { return index == -1; }
//	inline bool operator==(const NodeAccessor& other) { return type == other.type && index == other.index; }
//	inline static NodeAccessor getRoot() { return NodeAccessor(-1, 0); }
//	inline static NodeAccessor getNull() { return NodeAccessor(0, -1); }
//};

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
	
public:
	friend class GUIManager;
	friend class NodeCPU;
	friend class GPUParamData;  friend class GPUNodeData;

	static ivec2 opNodeByteSize;
	static ivec2 primNodeByteSize;

	ObjectManager(SDYShader*);

	NodeCPU* root; // always a MIN OP

	static string getDefaultName(SDNodeType);

	NodeCPU* addObject(NodeCPU* parent, SDNodeType, vec3 pos, vec3 euler);
	NodeCPU* addOperation(NodeCPU* parent, SDNodeType type, vec3 pos, vec3 euler);

	static ivec2 getStructSize(SDNodeType);

	void init();
};