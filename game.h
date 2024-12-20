#pragma once

#include "camera.h"
#include "mygl.h"

class Game {
private:
	Camera camera;
	Shader* shader;
public:
	Game(InputBundle*, Shader*);

	void init();
	void update(float dt);
};