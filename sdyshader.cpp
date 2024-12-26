#include "sdyshader.h"
#include "objectmanager.h"

SDYShader::SDYShader() : sphereUBOHandle(), Shader() {}

void SDYShader::setupObjectUBOs() {
	uint uboSpheresIndex = glGetUniformBlockIndex(programHandle, "SpheresBlock");
	uint uboOperationsIndex = glGetUniformBlockIndex(programHandle, "OperationsBlock");
	glUniformBlockBinding(programHandle, uboSpheresIndex, 0);
	glUniformBlockBinding(programHandle, uboOperationsIndex, 1);

	glGenBuffers(1, &sphereUBOHandle);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, sphereUBOHandle);
	glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * sizeof(vec4), NULL, GL_STATIC_DRAW);
	// https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL slightly different from this hope its ok

	glGenBuffers(1, &operationsUBOHandle);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, operationsUBOHandle);
	glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * sizeof(vec4), NULL, GL_STATIC_DRAW);

	uniformOperationCount(0);
}

void SDYShader::uniformOperationCount(int newOperationCount) {
	use();
	uniformInt("u_OperationCount", newOperationCount);
}

void SDYShader::setObject(int i, SDNodeType type, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, sphereUBOHandle);
	ivec2 sizeData = ObjectManager::getStructSize(type);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeData.y * i, sizeData.x, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0); // TODO: do something to prevent constant buffer switching?
}

void SDYShader::setOperation(int i, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, operationsUBOHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) * i, 12, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}