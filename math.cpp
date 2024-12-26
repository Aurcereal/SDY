#include "math.h"

void decomposeTransformation(const mat4& m, vec3* translation, vec3* euler) {
	// Translation
	*translation = vec3(m[3]);

	// Euler (assume XYZ order)
	euler->y = asin(m[0][2]);
	if (cos(euler->y) > 0.0001f) {
		euler->z = atan2(-m[0][1], m[0][0]);
		euler->x = atan2(-m[1][2], m[2][2]);
	}
	else {
		euler->z = 0.0f;
		euler->x = atan2(m[1][0], m[1][1]);
	}
}

//void createTransformation(vec3 translation, vec3 eulerXYZ) {
//	glm::euler
//}

//mat4 eulerToRotationXYZ(vec3 euler) {
//	return mat4(vec4(cos(euler.y)*cos(euler.z), cos(euler.x)*sin
//}