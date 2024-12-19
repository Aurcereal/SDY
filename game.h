#pragma once

#include "camera.h"

class Game {
private:
	Camera camera;
public:
	Game(InputBundle*);
	void update(float dt);
};