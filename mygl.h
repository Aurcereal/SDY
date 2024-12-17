#pragma once

#include "helperinclude.h"
#include "shader.h"
#include "quadrangle.h"

class MyGL {
private:
	GLuint vaoHandle;

	// TODO: temp remove
	Shader shader;
	Quadrangle q;
public:
	MyGL();
	~MyGL();
	void initializeGL();
	void render();
};