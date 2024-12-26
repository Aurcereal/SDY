#include "game.h"
#include "sdydatadefines.glsl"

Game::Game(GLFWwindow* window, InputBundle* input) : window(window), cameraEnabled(true), input(input), camera(input), gl(), objectManager(&gl.shader), guiManager(&objectManager) {}

void Game::init() {
	gl.initializeGL();

	camera.setInitialUniforms(&gl.shader);

	objectManager.addOperation(-1, OP_MIN);
	objectManager.addSphere(0, vec3(0.0f, 5.0f, 0.0f), vec3(0.0f), 0.5f);
	objectManager.addSphere(0, vec3(2.0f, 5.0f, 0.0f), vec3(0.0f), 0.5f);
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
	guiManager.drawImGuiElements(camera);
}