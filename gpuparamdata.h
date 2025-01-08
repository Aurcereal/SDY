#pragma once

#include "sdydatadefines.glsl"
#include "helperinclude.h"
#include <unordered_map>
#include <vector>

class ObjectManager;

struct Sphere {
	float r;
	inline Sphere(float r) : r(r) {}
	inline Sphere() : Sphere(1.0f) {}
};
struct Box {
	vec3 dim;
	inline Box(vec3 dim) : dim(dim) {}
	inline Box() : dim(1.0f) {}
};
struct Min {
	inline Min() {}
};
struct Max {
	inline Max() {}
};
struct SMin {
	float smoothness;
	inline SMin() : smoothness(0.5f) {}
};
struct SMax {
	float smoothness;
	inline SMax() : smoothness(0.5f) {}
};

class GPUParamData {
private:
	ObjectManager* objectManager;

	vector<Min> minOps;
	vector<Max> maxOps;
	vector<SMin> sminOps;
	vector<SMax> smaxOps;
	vector<Sphere> spheres;
	vector<Box> boxes;

	dict<SDNodeType, int> paramCount;

public:
	inline GPUParamData(ObjectManager* objectManager) : objectManager(objectManager) {}

	void setParamData(SDNodeType type, int i, void* data);
	void* getParamData(SDNodeType type, int i);
	int pushDefaultParamData(SDNodeType type);
	inline int getCount(SDNodeType type) { if (paramCount.count(type) == 0) paramCount[type] = 0; return paramCount[type]; }
};
