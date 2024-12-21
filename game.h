#pragma once

#include "camera.h"
#include "mygl.h"
#include "objectmanager.h"

class Game {
private:
	GLFWwindow* window;

	bool cameraEnabled;

	InputBundle* input;
	Camera camera;
	MyGL gl;
	ObjectManager objectManager;
public:
	Game(GLFWwindow*, InputBundle*);

	void init();
	void update(float dt);
	void render();
};