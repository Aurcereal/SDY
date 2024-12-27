#include "editableentity.h"
#include <glm/gtx/euler_angles.hpp>

vec3 EulerEntity::getEuler() {
	return euler;
}

void EulerEntity::setEuler(vec3 newEuler) {
	euler = newEuler;
	mat4 rot = glm::eulerAngleZYX(euler.z, euler.y, euler.x);
	ri = vec3(rot[0]);
	up = vec3(rot[1]);
	fo = vec3(rot[2]);
}

vec3 EulerEntity::getPos() {
	return position;
}

void EulerEntity::setPos(vec3 pos) {
	position = pos;
}