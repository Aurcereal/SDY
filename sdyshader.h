#pragma once

#include "shader.h"
#include "sdydatadefines.glsl"

using namespace std;

class SDYShader : public Shader {
private:
	dict<SDNodeType, uint> objectUBOHandles;
	dict<SDNodeType, int> objectCounts;
	static dict<SDNodeType, string> primToCountUniform;

	uint operationsUBOHandle;
	int operationCount;

	void uniformOperationCount(int);
	void uniformObjectCount(SDNodeType, int);
public:
	SDYShader();
	void setupObjectUBOs();

	void setObject(int i, SDNodeType, void* data, bool uniformCount = true);
	void setOperation(int i, void* data, bool uniformCount = true);
};