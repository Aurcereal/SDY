#include "entity.h"
#include <iostream>

mat4 Entity::getTransform() {
	std::cout << "mat4 mat3 constructor ok?" << endl;
	return glm::translate(mat4(), position) * mat4(mat3(ri, up, fo));
}