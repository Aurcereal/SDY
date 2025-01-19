#include "eulerentity.h"
#include <glm/gtx/euler_angles.hpp>
#include "imguizmo/ImGuizmo.h"

void EulerEntity::setEuler(vec3 newEuler) {
	euler = newEuler;
	mat4 rot = glm::eulerAngleZYX(euler.z, euler.y, euler.x);
	ri = vec3(rot[0]);
	up = vec3(rot[1]);
	fo = vec3(rot[2]);
}

void EulerEntity::setTransform(const mat4& transform) {
	vec3 p, e, s;
	ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(p), glm::value_ptr(e), glm::value_ptr(s));
	setPos(p); setEuler(e); setScale(s);
}