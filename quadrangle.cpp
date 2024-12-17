#include "quadrangle.h"

Quadrangle::Quadrangle() : Drawable() {}

void Quadrangle::createVBOData() {
	vector<vec4> positions = {
		vec4(-1.0f, -1.0f, 0.5f, 1.0f),
		vec4(-1.0f, 1.0f, 0.5f, 1.0f),
		vec4(1.0f, -1.0f, 0.5f, 1.0f),
		vec4(1.0f, 1.0f, 0.5f, 1.0f)
	};

	vector<GLuint> indices = {
		0, 2, 3,
		3, 1, 0
	};

	generateBuffer(POSITION);
	bindBuffer(POSITION);
	bufferData(POSITION, positions.data(), positions.size() * sizeof(vec4));

	generateBuffer(INDEX);
	bindBuffer(INDEX);
	bufferData(INDEX, indices.data(), indices.size() * sizeof(GLuint));

	indexCount = indices.size();
}