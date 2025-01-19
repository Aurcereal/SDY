#pragma once

#include "helperinclude.h"

class Entity {
protected:
	vec3 position;
	vec3 fo, ri, up;
	vec3 scale;
public:
	Entity();
	mat4 getTransform() const;
	mat4 getInverseTransform() const;
};