#include "sdyshader.h"
#include "objectmanager.h"

SDYShader::SDYShader() : paramUBOHandles(), opNodesUBOHandle(), primNodesUBOHandle(), spopNodesUBOHandle(), Shader() {}

dict<SDNodeType, string> SDYShader::paramToCountUniform = {
	{OP_MIN, "u_MinCount"},
	{OP_MAX, "u_MaxCount"},
	{OP_SMIN, "u_SMinCount"},
	{OP_SMAX, "u_SMaxCount"},

	{PRIM_SPHERE, "u_SphereCount"},
	{PRIM_BOX, "u_BoxCount"},

	{SPOP_TWIST, "u_TwistCount"},
	{SPOP_MIRROR, "u_MirrorCount"},
	{SPOP_REPEAT, "u_RepeatCount"},
	{SPOP_PINCH, "u_PinchCount"},
};

dict<SDNodeType, string> SDYShader::paramToBlockName = {
	{OP_MIN, "MinsBlock"},
	{OP_MAX, "MaxesBlock"},
	{OP_SMIN, "SMinsBlock"},
	{OP_SMAX, "SMaxesBlock"},

	{PRIM_SPHERE, "SpheresBlock"},
	{PRIM_BOX, "BoxesBlock"},

	{SPOP_TWIST, "TwistsBlock"},
	{SPOP_MIRROR, "MirrorsBlock"},
	{SPOP_REPEAT, "RepeatsBlock"},
	{SPOP_PINCH, "PinchesBlock"},
};

string SDYShader::opNodesBlockName = "OpNodesBlock";
string SDYShader::primNodesBlockName = "PrimNodesBlock";
string SDYShader::spopNodesBlockName = "SpopNodesBlock";

array<SDNodeType, 10> SDYShader::paramTypes = {
	OP_MIN, OP_MAX, OP_SMIN, OP_SMAX,
	PRIM_SPHERE, PRIM_BOX,
	SPOP_TWIST, SPOP_MIRROR, SPOP_REPEAT, SPOP_PINCH
};

void SDYShader::setupObjectUBOs() {
	uint blockInd;
	int currPlacementIndex = 0;

	blockInd = glGetUniformBlockIndex(programHandle, opNodesBlockName.c_str());
	glUniformBlockBinding(programHandle, blockInd, currPlacementIndex);
	glGenBuffers(1, &opNodesUBOHandle);
	glBindBufferBase(GL_UNIFORM_BUFFER, currPlacementIndex, opNodesUBOHandle);
	glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * GPUNodeData::opNodeByteSize.y, NULL, GL_STATIC_DRAW);
	currPlacementIndex++;

	blockInd = glGetUniformBlockIndex(programHandle, primNodesBlockName.c_str());
	glUniformBlockBinding(programHandle, blockInd, currPlacementIndex);
	glGenBuffers(1, &primNodesUBOHandle);
	glBindBufferBase(GL_UNIFORM_BUFFER, currPlacementIndex, primNodesUBOHandle);
	glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * GPUNodeData::primNodeByteSize.y, NULL, GL_STATIC_DRAW);
	currPlacementIndex++;

	blockInd = glGetUniformBlockIndex(programHandle, spopNodesBlockName.c_str());
	glUniformBlockBinding(programHandle, blockInd, currPlacementIndex);
	glGenBuffers(1, &spopNodesUBOHandle);
	glBindBufferBase(GL_UNIFORM_BUFFER, currPlacementIndex, spopNodesUBOHandle);
	glBufferData(GL_UNIFORM_BUFFER, ELEMCOUNT * GPUNodeData::spopNodeByteSize.y, NULL, GL_STATIC_DRAW);
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

void SDYShader::uniformOpNodeCount(int count) {
	use();
	uniformInt("u_OpNodeCount", count);
}

#include <iostream>

void SDYShader::uniformSpopNodeCount(int count) {
	use();
	uniformInt("u_SpopNodeCount", count);
}

void SDYShader::uniformParamCount(SDNodeType type, int count) {
	use();
	uniformInt(paramToCountUniform[type], count);
}

void SDYShader::setParamData(int i, SDNodeType type, void* data) {
	assert(paramUBOHandles.count(type) != 0);
	glBindBuffer(GL_UNIFORM_BUFFER, paramUBOHandles[type]);
	ivec2 sizeData = ObjectManager::getStructSize(type);
	if (type != OP_MIN && type != OP_MAX && type != SPOP_MIRROR) { // TODO: Temp because they have no data to buffer causes 1281 err
		glBufferSubData(GL_UNIFORM_BUFFER, sizeData.y * i, sizeData.x, data);
	}
	printGLErrorLog();
	glBindBuffer(GL_UNIFORM_BUFFER, 0); // TODO: do something to prevent constant buffer switching? maybe negligible
}

void SDYShader::setOpNodeData(int i, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, opNodesUBOHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, GPUNodeData::opNodeByteSize.y * i, GPUNodeData::opNodeByteSize.x, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SDYShader::setPrimNodeData(int i, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, primNodesUBOHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, GPUNodeData::primNodeByteSize.y * i, GPUNodeData::primNodeByteSize.x, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SDYShader::setSpopNodeData(int i, void* data) {
	glBindBuffer(GL_UNIFORM_BUFFER, spopNodesUBOHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, GPUNodeData::spopNodeByteSize.y * i, GPUNodeData::spopNodeByteSize.x, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

