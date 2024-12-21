#include "sdyshader.h"

SDYShader::SDYShader() : sphereUBOHandle(), sphereCount(0), Shader() {}

void SDYShader::setupObjectUBOs() {
	uint uboSpheresIndex = glGetUniformBlockIndex(programHandle, "SpheresBlock");// u_Spheres");
	glUniformBlockBinding(programHandle, uboSpheresIndex, 0);

	glGenBuffers(1, &sphereUBOHandle);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, sphereUBOHandle);
	glBufferData(GL_UNIFORM_BUFFER, 256 * sizeof(vec4), NULL, GL_STATIC_DRAW);
	// https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL slightly different from this hope its ok

	//
	updateSphereCount(sphereCount);
}

void SDYShader::updateSphereCount(int newSphereCount) {
	sphereCount = newSphereCount;
	use();
	uniformInt("u_SphereCount", sphereCount);
}

void SDYShader::setSphere(int i, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, sphereUBOHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) * i, sizeof(vec4), data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0); // TODO: do something to prevent constant buffer switching?
}