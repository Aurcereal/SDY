#include "drawable.h"

Drawable::Drawable() : indexCount() {}
Drawable::~Drawable() {
	destroyVBOData();
}

static dict<BufferType, bool> useElemArray = {
	{INDEX, true},
	{POSITION, false}
};

void Drawable::destroyVBOData() {
	for (auto kvp : buffers) {
		glDeleteBuffers(1, &kvp.second);
	}
	buffers.clear();
}

GLenum Drawable::drawMode() {
	return GL_TRIANGLES;
}

void Drawable::generateBuffer(BufferType type) {
	glGenBuffers(1, &buffers[type]);
}

bool Drawable::hasBuffer(BufferType type) {
	return buffers.count(type) != 0;
}

void Drawable::bindBuffer(BufferType type) {
	if (useElemArray.at(type)) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[type]);
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[type]);
	}
}

void Drawable::bufferData(BufferType type, void* data, GLuint size) {
	glBufferData(useElemArray.at(type) ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void Drawable::deleteBuffer(BufferType type) {
	glDeleteBuffers(1, &buffers[type]);
	buffers.erase(type);
}