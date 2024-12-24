#include "objectmanager.h"

Sphere::Sphere(vec3 p, float r) : pos(p), r(r) {}

void ObjectManager::addObject(SDYShader* shader, int parentIndex, SDNodeType type, void* data) {
	assert(type >= 0);

	switch (type) {
	case PRIM_SPHERE:
		spheres.push_back(*((Sphere*)data));
		shader->setSphere(spheres.size() - 1, data);
		break;
	}

	addOperation(shader, parentIndex, type);
}

void ObjectManager::addOperation(SDYShader* shader, int parentIndex, SDNodeType type) {
	operations.push_back(OperationNode());
	OperationNode& n = operations[operations.size() - 1];
	n.parentIndex = parentIndex;
	n.operationType = type;
	n.objectIndex = spheres.size() - 1;

	shader->setOperation(operations.size() - 1, &n);
	shader->uniformOperationCount(operations.size());
}