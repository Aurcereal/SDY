#pragma once

#include "helperinclude.h"
#include <vector>
#include "sdyshader.h"
#include "sdydatadefines.glsl"
#include <array>

struct Sphere {
	vec4 data;
};

struct OperationNode {
	int operationType;
	int childIndex1;
	int childIndex2;
};

struct SDYData {
	array<vec4, ELEMCOUNT> spheres;
	array<OperationNode, ELEMCOUNT> operations;

	int sphereCount;
	int operationCount;
};

class ObjectManager {
private:
	uPtr<SDYData> sdyData;
public:
	ObjectManager();
	void addSphere(SDYShader*, const vec4&); // most sd objects are very tiny so who cares copy
	void addOperation(SDYShader*, const OperationNode&); // temp, don't add operations raw this way
};