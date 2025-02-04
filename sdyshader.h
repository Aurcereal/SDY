#pragma once

#include "shader.h"
#include "sdy.datadefines.glsl"
#include <array>

using namespace std;

class SDYShader : public Shader {
private:
	dict<SDNodeType, uint> paramUBOHandles;
	uint opNodesUBOHandle;
	uint primNodesUBOHandle;

	static dict<SDNodeType, string> paramToCountUniform;
	static dict<SDNodeType, string> paramToBlockName;
	static string opNodesBlockName;
	static string primNodesBlockName;
	static array<SDNodeType, 6> paramTypes;

	
public:
	SDYShader();
	void setupObjectUBOs();

	void setParamData(int i, SDNodeType, void* data);
	void setOpNodeData(int i, void* data);
	void setPrimNodeData(int i, void* data);

	void uniformOpNodeCount(int);
	void uniformParamCount(SDNodeType, int);
};