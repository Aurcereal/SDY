#include "game.h"
#include "sdydatadefines.glsl"

Game::Game(GLFWwindow* window, InputBundle* input) : window(window), cameraEnabled(true), input(input), camera(input), gl(), objectManager(&gl.shader), guiManager(&objectManager) {}

void Game::init() {
	gl.initializeGL();
	objectManager.init();

	camera.setInitialUniforms(&gl.shader);

	NodeCPU* un = objectManager.addOperation(objectManager.root, OP_MIN, vec3(0.0f), vec3(0.0f), vec3(1.0f));
	objectManager.addObject(nullptr, PRIM_SPHERE, vec3(0.0f, 5.0f, 0.0f), vec3(1.0f, 0.0f,0.0f), vec3(1.0f));
	for(int i=0; i<50; i++)
		objectManager.addObject(un, PRIM_SPHERE, vec3(2.0f+i, 5.0f, 0.0f), vec3(0.0f), vec3(1.0f));
}

void Game::update(float dt) {
	if (input->f3Down) {
		cameraEnabled = !cameraEnabled;
		if(cameraEnabled)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		guiManager.setWidgetsActive(!cameraEnabled);
	}

	camera.update(dt, cameraEnabled);

	gl.shader.uniformVec2i("u_ScreenDimensions", MyGL::screenDimensions);
	camera.updateUniforms(&gl.shader);
}

void Game::render() {
	gl.render();
	guiManager.processInput(*input);
	guiManager.drawImGuiElements(camera);
}