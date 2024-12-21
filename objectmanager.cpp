#include "objectmanager.h"

void ObjectManager::addSphere(SDYShader* shader, const vec4 &sphere) {
	spheres.push_back(sphere);
	shader->setSphere(spheres.size() - 1, &spheres[spheres.size() - 1]);
	shader->updateSphereCount(spheres.size());
}