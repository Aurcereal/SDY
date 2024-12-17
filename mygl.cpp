#include "mygl.h"

MyGL::MyGL() : vaoHandle() {}

MyGL::~MyGL() {
	glDeleteVertexArrays(1, &vaoHandle);
}

void MyGL::initializeGL() {
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	shader.createAndCompileShaderProgram("default.vert", "default.frag");
	q.createVBOData();
}

void MyGL::render() {
	glClearColor(0.8, 0.4, 0.2, 1.0f); // Set current clear color
	glClear(GL_COLOR_BUFFER_BIT); // Clear color buffer with current clear col

	shader.draw(q);
}