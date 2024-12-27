#pragma once

#include "entity.h"

class EulerEntity : public Entity {
private:
	vec3 euler; // ZYX
public:
	vec3 getEuler();
	void setEuler(vec3);

	vec3 getPos();
	void setPos(vec3);
};