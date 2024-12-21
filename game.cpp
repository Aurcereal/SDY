#include "game.h"

Game::Game(GLFWwindow* window, InputBundle* input) : window(window), cameraEnabled(true), input(input), camera(input), gl() {}

void Game::init() {
	gl.initializeGL();

	camera.setInitialUniforms(&gl.shader);

	objectManager.addSphere(&gl.shader, vec4(0.0f, 10.0f, 0.0f, 1.0f));
}

void Game::update(float dt) {
	if (input->f3Down) {
		cameraEnabled = !cameraEnabled;
		if(cameraEnabled)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if(cameraEnabled)
		camera.update(dt);

	gl.shader.uniformVec2i("u_ScreenDimensions", MyGL::screenDimensions);
	camera.updateUniforms(&gl.shader);
}

void Game::render() {
	gl.render();
}