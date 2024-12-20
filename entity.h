#pragma once

#include "helperinclude.h"

class Entity {
protected:
	vec3 position;
	vec3 fo, ri, up;
public:
	Entity();
	mat4 getTransform();
};