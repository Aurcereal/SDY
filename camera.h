#pragma once

#include "inputbundle.h"
#include "entity.h"
#include "shader.h"

class Camera : public Entity {
private:
	InputBundle* input;
	vec2 prevMousePos;

	vec2 lookSens;
	float upBarrier;
	vec2 polarCoords;

	float fovY;

	vec3 moveSpeed;

	vec2 nearFar;
public:
	Camera(InputBundle*);
	void update(float dt, bool cameraEnabled);
	
	void setInitialUniforms(Shader*);
	void updateUniforms(Shader*);

	mat4 getProjectionMatrix() const;
};