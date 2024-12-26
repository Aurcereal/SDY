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

struct OperationNode {
	int parentIndex;

	int operationType;
	int objectIndex;
};

class ObjectManager {
private:
	vector<pair<Sphere, EulerEntity>> spheres;
	vector<OperationNode> operations;
	vector<vector<int>> childArray;

	static dict<SDNodeType, ivec2> sizeMap;

	pair<SDObject*, EulerEntity*> getObjectOfNode(int nodeIndex);

	SDYShader* shader;
public:
	ObjectManager(SDYShader*);

	friend class GUIManager;

	void addSphere(int parentIndex, vec3 pos, vec3 euler, float r); // will add the data and the node
	void addOperation(int parentIndex, SDNodeType type);

	static ivec2 getStructSize(SDNodeType);

	bool nodeContainsObject(int nodeIndex);
	
	void getTransformationOfNode(int nodeIndex, vec3* translation, vec3* euler, mat4* transform);
	void setTranslationEulerOfNode(int nodeIndex, vec3 translation, vec3 euler);
	void setTranslationOfNode(int nodeIndex, vec3 translation);
	void setEulerOfNode(int nodeIndex, vec3 euler);
};