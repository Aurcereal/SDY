#include "objectmanager.h"
#include "imguizmo/ImGuizmo.h"

ObjectManager::ObjectManager(SDYShader* shader) : shader(shader) {}

dict<SDNodeType, ivec2> ObjectManager::sizeMap = {
	{PRIM_SPHERE, ivec2(18 * 4, 20 * 4)},
	{PRIM_BOX, ivec2(23 * 4, 24 * 4) }
};

dict<SDNodeType, string> ObjectManager::classMap = {
	{PRIM_SPHERE, "Sphere"},
	{PRIM_BOX, "Box"},
	{OP_MIN, "Union"},
	{OP_SMIN, "Smooth Union"},
	{OP_MAX, "Intersection"},
	{OP_SMAX, "Smooth Intersection"}
};

ivec2 ObjectManager::getStructSize(SDNodeType type) {
	assert(sizeMap.count(type) != 0);
	return sizeMap[type];
}

SDObject::SDObject(mat4 m, int parentIndex) : invTransform(m), parentIndex(parentIndex) {}

Sphere::Sphere(mat4 m, int pi, float r) : SDObject(m, pi), r(r) {}
Box::Box(mat4 m, int pi, vec3 dim) : SDObject(m, pi), dim(dim) {}

void ObjectManager::addSphere(int parentIndex, vec3 pos, vec3 euler, float r) {
	spheres.push_back(pair<Sphere, EulerEntity>(Sphere(mat4(1.0f), parentIndex, r), EulerEntity()));
	setTranslationEulerOfNode(NodeAccessor(PRIM_SPHERE, spheres.size()-1), pos, euler);
	shader->setObject(spheres.size()-1, PRIM_SPHERE, &spheres[spheres.size()-1]);
	if (parentIndex != -1) childArray[parentIndex].push_back(NodeAccessor(PRIM_SPHERE, spheres.size()-1));
}

void ObjectManager::addBox(int parentIndex, vec3 pos, vec3 euler, vec3 dim) {
	boxes.push_back(pair<Box, EulerEntity>(Box(mat4(1.0f), parentIndex, dim), EulerEntity()));
	setTranslationEulerOfNode(NodeAccessor(PRIM_BOX, boxes.size() - 1), pos, euler);
	shader->setObject(boxes.size() - 1, PRIM_BOX, &boxes[boxes.size() - 1]);
	if (parentIndex != -1) childArray[parentIndex].push_back(NodeAccessor(PRIM_BOX, boxes.size() - 1));
}

void ObjectManager::addOperation(int parentIndex, SDNodeType type) {
	assert(type < 0);

	operations.push_back(OperationNode());
	childArray.push_back(vector<NodeAccessor>());
	int opIndex = operations.size() - 1;
	OperationNode& n = operations[opIndex];
	n.parentIndex = parentIndex;
	n.operationType = type;

	assert(parentIndex < opIndex);
	if(parentIndex != -1) childArray[parentIndex].push_back(NodeAccessor(type, opIndex));

	shader->setOperation(opIndex, &n);
}

bool ObjectManager::nodeContainsObject(NodeAccessor accessor) {
	return accessor.type >= 0;
}
pair<SDObject*, EulerEntity*> ObjectManager::getObjectOfNode(NodeAccessor accessor) {
	assert(nodeContainsObject(accessor));

	switch (accessor.type) {
	case PRIM_SPHERE:
		return pair<SDObject*, EulerEntity*>(&spheres[accessor.index].first, &spheres[accessor.index].second);
		break;
	case PRIM_BOX:
		return pair<SDObject*, EulerEntity*>(&boxes[accessor.index].first, &boxes[accessor.index].second);
		break;
	}

	throw std::exception();
}

void ObjectManager::getTransformationOfNode(NodeAccessor accessor, vec3* translation, vec3* euler, mat4* transform) {
	assert(nodeContainsObject(accessor));

	EulerEntity* e = getObjectOfNode(accessor).second;

	if (translation != nullptr) *translation = e->getPos();
	if (euler != nullptr) *euler = e->getEuler();
	if (transform != nullptr) *transform = e->getTransform();

}
void ObjectManager::setTranslationOfNode(NodeAccessor accessor, vec3 translation) {
	assert(nodeContainsObject(accessor));

	auto obj = getObjectOfNode(accessor);
	obj.second->setPos(translation);

	obj.first->invTransform = obj.second->getInverseTransform();
	shader->setObject(accessor.index, accessor.type, obj.first);
}
void ObjectManager::setEulerOfNode(NodeAccessor accessor, vec3 euler) {
	assert(nodeContainsObject(accessor));

	auto obj = getObjectOfNode(accessor);
	obj.second->setEuler(euler);

	obj.first->invTransform = obj.second->getInverseTransform();
	shader->setObject(accessor.index, accessor.type, obj.first);
}
void ObjectManager::setTranslationEulerOfNode(NodeAccessor accessor, vec3 translation, vec3 euler) {
	assert(nodeContainsObject(accessor));

	auto obj = getObjectOfNode(accessor);
	obj.second->setPos(translation);
	obj.second->setEuler(euler);

	obj.first->invTransform = obj.second->getInverseTransform();
	shader->setObject(accessor.index, accessor.type, obj.first);
}

string ObjectManager::getName(NodeAccessor accessor) {
	if (nameMap.count(accessorToKey(accessor)) != 0) {
		return nameMap[accessorToKey(accessor)];
	}
	else {
		assert(classMap.count(accessor.type) != 0);
		string s = classMap[accessor.type];
		return s.append(" ").append(std::to_string(accessor.index));
	}
}

void ObjectManager::setName(NodeAccessor accessor, string name) {
	nameMap[accessorToKey(accessor)] = name;
}