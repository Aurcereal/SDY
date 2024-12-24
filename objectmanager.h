#pragma once

#include "helperinclude.h"
#include <vector>
#include "sdyshader.h"
#include "sdydatadefines.glsl"
#include <array>

struct Sphere {
	vec3 pos;
	float r;
	Sphere(vec3, float);
};

struct OperationNode {
	int parentIndex;

	int operationType;
	int objectIndex;
};

class ObjectManager {
private:
	vector<Sphere> spheres;
	vector<OperationNode> operations;
	vector<vector<int>> childArray;
public:
	friend class GUIManager;

	void addObject(SDYShader*, int parentIndex, SDNodeType type, void* data); // will add the data and the node
	void addOperation(SDYShader*, int parentIndex, SDNodeType type);
};