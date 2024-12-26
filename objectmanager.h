#pragma once

#include "helperinclude.h"
#include <vector>
#include "sdyshader.h"
#include "sdydatadefines.glsl"
#include "editableentity.h"

struct SDObject {
	mat4 invTransform;
	SDObject(mat4);
};

struct Sphere : public SDObject {
	float r; // maybe remove radius when/if transform scale good
	Sphere(mat4, float);
};

struct Box : public SDObject {
	vec3 dim; // same idea as sphere where u could remove
	Box(mat4, vec3);
};

struct OperationNode {
	int parentIndex;

	int operationType;
	int objectIndex;
};

class ObjectManager {
private:
	vector<pair<Sphere, EulerEntity>> spheres;
	vector<pair<Box, EulerEntity>> boxes;

	vector<OperationNode> operations;
	vector<vector<int>> childArray;

	static dict<SDNodeType, ivec2> sizeMap;

	pair<SDObject*, EulerEntity*> getObjectOfNode(int nodeIndex);

	SDYShader* shader;
public:
	ObjectManager(SDYShader*);

	friend class GUIManager;

	void addSphere(int parentIndex, vec3 pos, vec3 euler, float r);
	void addBox(int parentIndex, vec3 pos, vec3 euler, vec3 dim);

	void addOperation(int parentIndex, SDNodeType type, int objectIndex = -1);

	static ivec2 getStructSize(SDNodeType);

	bool nodeContainsObject(int nodeIndex);
	
	void getTransformationOfNode(int nodeIndex, vec3* translation, vec3* euler, mat4* transform);
	void setTranslationEulerOfNode(int nodeIndex, vec3 translation, vec3 euler);
	void setTranslationOfNode(int nodeIndex, vec3 translation);
	void setEulerOfNode(int nodeIndex, vec3 euler);
};