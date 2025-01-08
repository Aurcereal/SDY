#include "objectmanager.h"
#include "imguizmo/ImGuizmo.h"

ivec2 ObjectManager::opNodeByteSize = ivec2(12, sizeof(vec4));
ivec2 ObjectManager::primNodeByteSize = ivec2(sizeof(vec4) * 5, sizeof(vec4) * 5);

ObjectManager::ObjectManager(SDYShader* shader) : shader(shader), root(nullptr), nodeData(this), paramData(this) {}

void ObjectManager::init() {
	root = addOperation(nullptr, OP_MIN, vec3(0.0f), vec3(0.0f));
}

dict<SDNodeType, ivec2> ObjectManager::byteSizeMap = {
	{OP_MIN, ivec2(0, sizeof(vec4))},
	{OP_MAX, ivec2(0, sizeof(vec4))},
	{OP_SMIN, ivec2(4, sizeof(vec4))},
	{OP_SMAX, ivec2(4, sizeof(vec4))},

	{PRIM_SPHERE, ivec2(4, 16)},
	{PRIM_BOX, ivec2(12, 16) }
};

dict<SDNodeType, string> ObjectManager::defaultNameMap = {
	{PRIM_SPHERE, "Sphere"},
	{PRIM_BOX, "Box"},
	{OP_MIN, "Union"},
	{OP_SMIN, "Smooth Union"},
	{OP_MAX, "Intersection"},
	{OP_SMAX, "Smooth Intersection"}
};

ivec2 ObjectManager::getStructSize(SDNodeType type) {
	assert(byteSizeMap.count(type) != 0);
	return byteSizeMap[type];
}

NodeCPU* ObjectManager::addObject(NodeCPU* parent, SDNodeType type, vec3 pos, vec3 euler) {
	parent = parent == nullptr ? root : parent;

	nodes.push_back(mkU<NodeCPU>(this, parent, type));
	NodeCPU* node = nodes[nodes.size() - 1].get();
	parent->addChild(node);

	node->setLocalPosEuler(pos, euler);

	return node;
}
NodeCPU* ObjectManager::addOperation(NodeCPU* parent, SDNodeType type, vec3 pos, vec3 euler) {
	assert(type < 0);

	nodes.push_back(mkU<NodeCPU>(this, parent, type));
	NodeCPU* node = nodes[nodes.size() - 1].get();
	if(parent != nullptr) parent->addChild(node);

	node->setLocalPosEuler(pos, euler);

	return node;
}

string ObjectManager::getDefaultName(SDNodeType type) {
	assert(defaultNameMap.count(type) != 0);
	return defaultNameMap[type];
}