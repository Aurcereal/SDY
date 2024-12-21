#pragma once

#include "camera.h"
#include "mygl.h"
#include "objectmanager.h"

class Game {
private:
	Camera camera;
	MyGL gl;
	ObjectManager objectManager;
public:
	Game(InputBundle*);

	void init();
	void update(float dt);
	void render();
};