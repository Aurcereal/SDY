#include "camera.h"
#include <iostream>
#include "mygl.h"

Camera::Camera(InputBundle* input) : input(input), prevMousePos(input->mousePos), lookSens(vec2(0.005f)), upBarrier(0.1f), fovY(PI * 0.5f), moveSpeed(2.0f), polarCoords(0.0f), nearFar(0.1f, 1000.0f) {}

void Camera::update(float dt) {

	// Orientation
	vec2 dm = (input->mousePos - prevMousePos) * vec2(1.0f, -1.0f);
	if (prevMousePos.x == -1.0f) dm *= 0.0f; // Ignore first mouse callback
	prevMousePos = input->mousePos;

	polarCoords += dm * lookSens;
	if(abs(polarCoords.y) > 0.0f) polarCoords.y = sign(polarCoords.y) * glm::min(abs(polarCoords.y), PI * 0.5f - upBarrier);

	//
	fo = cos(polarCoords.y) * vec3(sin(polarCoords.x), 0.0f, cos(polarCoords.x)) + sin(polarCoords.y) * vec3(0.0f, 1.0f, 0.0f);
	up = normalize(vec3(0.0f, 1.0f, 0.0f) - fo * fo.y);
	ri = cross(up, fo);

	//
	vec4 move = vec4(vec3(int(input->d) - int(input->a), int(input->e) - int(input->q), int(input->w) - int(input->s)) * moveSpeed * dt, 0.0f);
	position += vec3(getTransform() * move);

}

void Camera::setInitialUniforms(Shader* shader) {
	shader->use();
	shader->uniformFloat("u_fovY", fovY);
	updateUniforms(shader);
}
void Camera::updateUniforms(Shader* shader) {
	shader->use();
	shader->uniformVec3("u_CamPos", position);
	shader->uniformVec3("u_CamRi", ri);
	shader->uniformVec3("u_CamUp", up);
	shader->uniformVec3("u_CamFo", fo);
}

mat4 Camera::getProjectionMatrix() const {
	float aspect = (1.0f * MyGL::screenDimensions.x) / MyGL::screenDimensions.y;
	float mi = 1.0f / tan(fovY * 0.5f);
	return mat4(vec4(mi / aspect, 0.0f, 0.0f, 0.0f), vec4(0.0f, mi, 0.0f, 0.0f), vec4(0.0f, 0.0f, nearFar.y/(nearFar.y-nearFar.x), 1.0f), vec4(0.0f, 0.0f, -nearFar.y*nearFar.x/(nearFar.y-nearFar.x), 0.0f));
}