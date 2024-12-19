#pragma once

#include "entity.h"

class Camera : public Entity {
private:
	InputBundle* input;
	vec2 prevMousePos;
public:
	Camera(InputBundle*);
	void update();
};