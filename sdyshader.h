#pragma once

#include "shader.h"

// delete this comment please actually: my idea rn is that this class doesn't know much and has low level interface for objectmanager to use
class SDYShader : public Shader {
private:
	uint sphereUBOHandle;
	int sphereCount;
public:
	SDYShader();
	void setupObjectUBOs();

	void updateSphereCount(int);
	void setSphere(int i, void* data);
};