#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void decomposeTransformation(const mat4& m, vec3* translation, vec3* euler);

mat4 eulerToRotationXYZ(vec3 euler);