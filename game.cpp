#include "game.h"

Game::Game(InputBundle* input, Shader* shader) : camera(input), shader(shader) {}

void Game::init() {
	camera.setInitialUniforms(shader);
}

void Game::update(float dt) {
	camera.update(dt);

	shader->uniformVec2i("u_ScreenDimensions", MyGL::screenDimensions);
	camera.updateUniforms(shader);
}