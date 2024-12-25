#include "game.h"
#include "sdydatadefines.glsl"

Game::Game(GLFWwindow* window, InputBundle* input) : window(window), cameraEnabled(true), input(input), camera(input), gl(), guiManager(&objectManager) {}

void Game::init() {
	gl.initializeGL();

	camera.setInitialUniforms(&gl.shader);

	Sphere s1 = Sphere(vec3(0.0f, 5.0f, 0.0f), 0.5f);
	Sphere s2 = Sphere(vec3(2.0f, 5.0f, 0.0f), 0.5f);

	objectManager.addOperation(&gl.shader, -1, OP_MIN);
	objectManager.addObject(&gl.shader, 0, PRIM_SPHERE, &s1);
	objectManager.addObject(&gl.shader, 0, PRIM_SPHERE, &s2);
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

	if(cameraEnabled)
		camera.update(dt);

	gl.shader.uniformVec2i("u_ScreenDimensions", MyGL::screenDimensions);
	camera.updateUniforms(&gl.shader);
}

void Game::render() {
	gl.render();
	guiManager.drawImGuiElements(camera);
}