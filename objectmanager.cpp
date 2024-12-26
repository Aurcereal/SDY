#include "objectmanager.h"

ObjectManager::ObjectManager(SDYShader* shader) : shader(shader) {}

dict<SDNodeType, ivec2> ObjectManager::sizeMap = {
	{PRIM_SPHERE, ivec2(17 * 4, 20 * 4)}
};

ivec2 ObjectManager::getStructSize(SDNodeType type) {
	assert(sizeMap.count(type) != 0);
	return sizeMap[type];
}

SDObject::SDObject(mat4 m) : invTransform(m) {}

Sphere::Sphere(mat4 m, float r) : SDObject(m), r(r) {}

void ObjectManager::addSphere(int parentIndex, vec3 pos, vec3 euler, float r) {
	spheres.push_back(pair<Sphere, EulerEntity>(Sphere(mat4(1.0f), r), EulerEntity()));
	int ind = spheres.size() - 1;

	EulerEntity& e = spheres[ind].second;
	Sphere& s = spheres[ind].first;

	e.setEuler(euler);
	e.setPos(pos);
	s.invTransform = e.getInverseTransform();

	shader->setObject(ind, PRIM_SPHERE, &s);
	addOperation(parentIndex, PRIM_SPHERE);
}

void ObjectManager::addOperation(int parentIndex, SDNodeType type) {
	operations.push_back(OperationNode());
	childArray.push_back(vector<int>());
	int opIndex = operations.size() - 1;
	OperationNode& n = operations[opIndex];
	n.parentIndex = parentIndex;
	n.operationType = type;
	n.objectIndex = spheres.size() - 1;

	assert(parentIndex < opIndex);
	if(parentIndex != -1) childArray[parentIndex].push_back(opIndex);

	shader->setOperation(opIndex, &n);
	shader->uniformOperationCount(operations.size());
}

bool ObjectManager::nodeContainsObject(int nodeIndex) {
	return operations[nodeIndex].operationType >= 0;
}
pair<SDObject*, EulerEntity*> ObjectManager::getObjectOfNode(int nodeIndex) {
	assert(nodeContainsObject(nodeIndex));

	auto& operation = operations[nodeIndex];
	switch (operation.operationType) {
	case PRIM_SPHERE:
		return pair<SDObject*, EulerEntity*>(&spheres[operation.objectIndex].first, &spheres[operation.objectIndex].second);
		break;
	}

	throw std::exception();
}

void ObjectManager::getTransformationOfNode(int nodeIndex, vec3* translation, vec3* euler, mat4* transform) {
	assert(nodeContainsObject(nodeIndex));

	EulerEntity* e = getObjectOfNode(nodeIndex).second;

	if (translation != nullptr) *translation = e->getPos();
	if (euler != nullptr) *euler = e->getEuler();
	if (transform != nullptr) *transform = e->getTransform();
}
void ObjectManager::setTranslationOfNode(int nodeIndex, vec3 translation) {
	assert(nodeContainsObject(nodeIndex));

	auto obj = getObjectOfNode(nodeIndex);
	obj.second->setPos(translation);

	obj.first->invTransform = obj.second->getInverseTransform();
	shader->setObject(operations[nodeIndex].objectIndex, PRIM_SPHERE, obj.first);
}
void ObjectManager::setEulerOfNode(int nodeIndex, vec3 euler) {
	assert(nodeContainsObject(nodeIndex));

	auto obj = getObjectOfNode(nodeIndex);
	obj.second->setEuler(euler);

	obj.first->invTransform = obj.second->getInverseTransform();
	shader->setObject(operations[nodeIndex].objectIndex, PRIM_SPHERE, obj.first);
}
void ObjectManager::setTranslationEulerOfNode(int nodeIndex, vec3 translation, vec3 euler) {
	assert(nodeContainsObject(nodeIndex));

	auto obj = getObjectOfNode(nodeIndex);
	obj.second->setPos(translation);
	obj.second->setEuler(euler);

	obj.first->invTransform = obj.second->getInverseTransform();
	shader->setObject(operations[nodeIndex].objectIndex, PRIM_SPHERE, obj.first);
}