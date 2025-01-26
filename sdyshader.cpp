#include "sdyshader.h"
#include "objectmanager.h"

SDYShader::SDYShader() : paramUBOHandles(), opNodesUBOHandle(), primNodesUBOHandle(), Shader() {}

dict<SDNodeType, string> SDYShader::paramToCountUniform = {
	{OP_MIN, "u_MinCount"},
	{OP_MAX, "u_MaxCount"},
	{OP_SMIN, "u_SMinCount"},
	{OP_SMAX, "u_SMaxCount"},

	{PRIM_SPHERE, "u_SphereCount"},
	{PRIM_BOX, "u_BoxCount"}
};

dict<SDNodeType, string> SDYShader::paramToBlockName = {
	{OP_MIN, "MinsBlock"},
	{OP_MAX, "MaxesBlock"},
	{OP_SMIN, "SMinsBlock"},
	{OP_SMAX, "SMaxesBlock"},

	{PRIM_SPHERE, "SpheresBlock"},
	{PRIM_BOX, "BoxesBlock"}
};

string SDYShader::opNodesBlockName = "OpNodesBlock";
string SDYShader::primNodesBlockName = "PrimNodesBlock";

array<SDNodeType, 6> SDYShader::paramTypes = {
	OP_MIN, OP_MAX, OP_SMIN, OP_SMAX,
	PRIM_SPHERE, PRIM_BOX
};

void SDYShader::setupObjectUBOs() {
	uint blockInd;
	int currPlacementIndex = 0;

	blockInd = glGetUniformBlockIndex(programHandle, opNodesBlockName.c_str());
	glUniformBlockBinding(programHandle, blockInd, currPlacementIndex);
	glGenBuffers(1, &opNodesUBOHandle);
	glBindBufferBase(GL_UNIFORM_BUFFER, currPlacementIndex, opNodesUBOHandle);
	glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * ObjectManager::opNodeByteSize.y, NULL, GL_STATIC_DRAW);
	currPlacementIndex++;

	blockInd = glGetUniformBlockIndex(programHandle, primNodesBlockName.c_str());
	glUniformBlockBinding(programHandle, blockInd, currPlacementIndex);
	glGenBuffers(1, &primNodesUBOHandle);
	glBindBufferBase(GL_UNIFORM_BUFFER, currPlacementIndex, primNodesUBOHandle);
	glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * ObjectManager::primNodeByteSize.y, NULL, GL_STATIC_DRAW);
	currPlacementIndex++;

	for (SDNodeType type : paramTypes) {
		uint blockInd = glGetUniformBlockIndex(programHandle, paramToBlockName[type].c_str());
		glUniformBlockBinding(programHandle, blockInd, currPlacementIndex);
		glGenBuffers(1, &paramUBOHandles[type]);
		glBindBufferBase(GL_UNIFORM_BUFFER, currPlacementIndex, paramUBOHandles[type]);
		glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * ObjectManager::getStructSize(type).y, NULL, GL_STATIC_DRAW);
		currPlacementIndex++;
	}

	printGLErrorLog();
}

#include <iostream>

void SDYShader::uniformOpNodeCount(int count) {
	use();
	std::cout << "u_OpNodeCount: " << count << std::endl;
	uniformInt("u_OpNodeCount", count);
}

void SDYShader::uniformParamCount(SDNodeType type, int count) {
	use();
	uniformInt(paramToCountUniform[type], count);
}

void SDYShader::setParamData(int i, SDNodeType type, void* data) {
	assert(paramUBOHandles.count(type) != 0);
	glBindBuffer(GL_UNIFORM_BUFFER, paramUBOHandles[type]);
	printGLErrorLog();
	ivec2 sizeData = ObjectManager::getStructSize(type);
	if (type != OP_MIN && type != OP_MAX) { // TODO: Temp because they have no data to buffer causes 1281 err
		glBufferSubData(GL_UNIFORM_BUFFER, sizeData.y * i, sizeData.x, data);
	}
	printGLErrorLog();
	glBindBuffer(GL_UNIFORM_BUFFER, 0); // TODO: do something to prevent constant buffer switching?
}

void SDYShader::setOpNodeData(int i, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, opNodesUBOHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, ObjectManager::opNodeByteSize.y * i, ObjectManager::opNodeByteSize.x, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SDYShader::setPrimNodeData(int i, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, primNodesUBOHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, ObjectManager::primNodeByteSize.y * i, ObjectManager::primNodeByteSize.x, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}