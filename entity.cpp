#include "entity.h"
#include <iostream>

Entity::Entity() : ri(1.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f), fo(0.0f, 0.0f, 1.0f), position(0.0f) {}

mat4 Entity::getTransform() {
	//std::cout << "mat4 mat3 constructor ok?" << endl;
	//cout << position.z << endl;
	return glm::translate(mat4(1.0f), position)* mat4(vec4(ri, 0.0f), vec4(up, 0.0f), vec4(fo, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f));
}