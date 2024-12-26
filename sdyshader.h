#pragma once

#include "shader.h"
#include "sdydatadefines.glsl"

// delete this comment please actually: my idea rn is that this class doesn't know much and has low level interface for objectmanager to use
class SDYShader : public Shader {
private:
	dict<SDNodeType, uint> objectUBOHandles;
	uint operationsUBOHandle;
public:
	SDYShader();
	void setupObjectUBOs();

	void uniformOperationCount(int);

	void setObject(int i, SDNodeType, void* data);
	void setOperation(int i, void* data);
};