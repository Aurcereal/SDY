#pragma once

#include "entity.h"

class EulerEntity : public Entity {
private:
	vec3 euler; // ZYX
public:
	inline vec3 getEuler() const { return euler; }
	void setEuler(vec3);

	inline vec3 getPos() const { return position; }
	inline void setPos(vec3 pos) { position = pos; }

	inline vec3 getScale() const { return scale; }
	inline void setScale(vec3 sca) { scale = sca; }

	void setTransform(const mat4&);
};