#pragma once

#include "entity.h"

class EulerEntity : public Entity {
private:
	vec3 euler; // ZYX
public:
	vec3 getEuler() const;
	void setEuler(vec3);

	vec3 getPos() const;
	void setPos(vec3);

	void setTransform(const mat4&);
};