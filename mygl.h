#pragma once

#include "helperinclude.h"
#include "sdyshader.h"
#include "quadrangle.h"

class MyGL {
private:
	GLuint vaoHandle;

	// TODO: temp remove
	Quadrangle q;
public:
	static ivec2 screenDimensions;

	MyGL();
	~MyGL();
	void initializeGL();
	void render();

	// TODO: temp remove
	SDYShader shader;
};