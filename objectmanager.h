#pragma once

#include "helperinclude.h"
#include <vector>
#include "sdyshader.h"
#include "sdydatadefines.glsl"
#include "editableentity.h"

struct SDObject {
	mat4 invTransform;
	int parentIndex;
	SDObject(mat4, int);
};

struct Sphere : public SDObject {
	float r; // maybe remove radius when/if transform scale good
	Sphere(mat4, int, float);
};

struct Box : public SDObject {
	vec3 dummy; // temp data alignment fix, wanna rearrange variables in c++ even with inheritance but how
	vec3 dim; // same idea as sphere where u could remove
	Box(mat4, int, vec3);
};

struct OperationNode {
	int parentIndex;
	int operationType;
};

struct NodeAccessor {
	SDNodeType type;
	int index;
	inline NodeAccessor(SDNodeType type, int index) : type(type), index(index) {}
	inline bool isLeaf() { return type >= 0; }
	inline bool isNull() { return index == -1; }
	inline bool operator==(const NodeAccessor& other) { return type == other.type && index == other.index; }
	inline static NodeAccessor getRoot() { return NodeAccessor(-1, 0); }
	inline static NodeAccessor getNull() { return NodeAccessor(0, -1); }
};

class ObjectManager {
private:
	vector<pair<Sphere, EulerEntity>> spheres;
	vector<pair<Box, EulerEntity>> boxes;

	vector<OperationNode> operations;
	vector<vector<NodeAccessor>> childArray;

	static dict<SDNodeType, ivec2> sizeMap;

	pair<SDObject*, EulerEntity*> getObjectOfNode(NodeAccessor);

	SDYShader* shader;
public:
	ObjectManager(SDYShader*);

	friend class GUIManager;

	void addSphere(int parentIndex, vec3 pos, vec3 euler, float r);
	void addBox(int parentIndex, vec3 pos, vec3 euler, vec3 dim);

	void addOperation(int parentIndex, SDNodeType type);

	static ivec2 getStructSize(SDNodeType);

	bool nodeContainsObject(NodeAccessor);
	
	void getTransformationOfNode(NodeAccessor, vec3* translation, vec3* euler, mat4* transform);
	void setTranslationEulerOfNode(NodeAccessor, vec3 translation, vec3 euler);
	void setTranslationOfNode(NodeAccessor, vec3 translation);
	void setEulerOfNode(NodeAccessor, vec3 euler);
};