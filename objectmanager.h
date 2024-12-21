#pragma once

#include "helperinclude.h"
#include <vector>
#include "sdyshader.h"

struct Sphere {
	vec4 data;
};

class ObjectManager {
private:
	vector<vec4> spheres;
public:
	void addSphere(SDYShader*, const vec4&); // most sd objects are very tiny so who cares copy
};