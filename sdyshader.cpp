#include "sdyshader.h"
#include "objectmanager.h"

SDYShader::SDYShader() : objectUBOHandles(), objectCounts(), operationsUBOHandle(), operationCount(0), Shader() {}

dict<SDNodeType, string> SDYShader::primToCountUniform = {
	{PRIM_SPHERE, "u_SphereCount"},
	{PRIM_BOX, "u_BoxCount"}
};

void SDYShader::setupObjectUBOs() {
	objectUBOHandles[PRIM_SPHERE] = glGetUniformBlockIndex(programHandle, "SpheresBlock");
	objectUBOHandles[PRIM_BOX] = glGetUniformBlockIndex(programHandle, "BoxesBlock");
	uint uboOperationsIndex = glGetUniformBlockIndex(programHandle, "OperationsBlock");
	glUniformBlockBinding(programHandle, uboOperationsIndex, 2);
	glUniformBlockBinding(programHandle, objectUBOHandles[PRIM_SPHERE], 0);
	glUniformBlockBinding(programHandle, objectUBOHandles[PRIM_BOX], 1);
	
	// https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
	glGenBuffers(1, &operationsUBOHandle);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, operationsUBOHandle);
	glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * sizeof(vec4), NULL, GL_STATIC_DRAW);

	// turn into funcs later
	glGenBuffers(1, &objectUBOHandles[PRIM_SPHERE]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, objectUBOHandles[PRIM_SPHERE]);
	glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * ObjectManager::getStructSize(PRIM_SPHERE).y, NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &objectUBOHandles[PRIM_BOX]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, objectUBOHandles[PRIM_BOX]);
	glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * ObjectManager::getStructSize(PRIM_BOX).y, NULL, GL_STATIC_DRAW);

	uniformOperationCount(0);
}

void SDYShader::uniformOperationCount(int newOperationCount) {
	use();
	uniformInt("u_OperationCount", newOperationCount);
	operationCount = newOperationCount;
}

void SDYShader::uniformObjectCount(int newObjectCount, SDNodeType type) {
	use();
	assert(primToCountUniform.count(type) != 0);
	uniformInt(primToCountUniform[type], newObjectCount);
	objectCounts[type] = newObjectCount;
}

void SDYShader::setObject(int i, SDNodeType type, void* data, bool uniformCount) {
	assert(objectUBOHandles.count(type) != 0);
	glBindBuffer(GL_UNIFORM_BUFFER, objectUBOHandles[type]);
	ivec2 sizeData = ObjectManager::getStructSize(type);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeData.y * i, sizeData.x, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0); // TODO: do something to prevent constant buffer switching?

	if (uniformCount && i > objectCounts[type] - 1)
		uniformObjectCount(i + 1, type);
}

void SDYShader::setOperation(int i, void* data, bool uniformCount) {
	glBindBuffer(GL_UNIFORM_BUFFER, operationsUBOHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) * i, 8, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if (uniformCount && i > operationCount - 1)
		uniformOperationCount(i + 1);
}