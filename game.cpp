#include "game.h"
#include "sdydatadefines.glsl"

Game::Game(GLFWwindow* window, InputBundle* input) : window(window), cameraEnabled(true), input(input), camera(input), gl() {}

void Game::init() {
	gl.initializeGL();

	camera.setInitialUniforms(&gl.shader);

	objectManager.addSphere(&gl.shader, vec4(0.0f, 10.0f, 0.0f, 1.0f));
	objectManager.addSphere(&gl.shader, vec4(4.0f, 10.0f, 0.0f, 1.0f));
	
	OperationNode n1, n2, n3;
	n1.operationType = OP_MIN;
	n1.parentIndex = -1;
	n1.objectIndex = -1;
	n2.operationType = PRIM_SPHERE;
	n2.parentIndex = 0;
	n2.objectIndex = 0;
	n3.operationType = PRIM_SPHERE;
	n3.parentIndex = 0;
	n3.objectIndex = 1;
	
	objectManager.addOperation(&gl.shader, n1);
	objectManager.addOperation(&gl.shader, n2);
	objectManager.addOperation(&gl.shader, n3);
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