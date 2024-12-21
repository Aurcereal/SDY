#include "game.h"

Game::Game(InputBundle* input) : camera(input), gl() {}

void Game::init() {
	gl.initializeGL();

	camera.setInitialUniforms(&gl.shader);

	objectManager.addSphere(&gl.shader, vec4(0.0f, 10.0f, 0.0f, 1.0f));
}

void Game::update(float dt) {
	camera.update(dt);

	gl.shader.uniformVec2i("u_ScreenDimensions", MyGL::screenDimensions);
	camera.updateUniforms(&gl.shader);
}

void Game::render() {
	gl.render();
}