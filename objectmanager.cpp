#include "objectmanager.h"
#include "imguizmo/ImGuizmo.h"

ObjectManager::ObjectManager(SDYShader* shader) : shader(shader), root(nullptr), nodeData(this), paramData(this) {}

void ObjectManager::init() {
	root = addNode(nullptr, OP_MIN, vec3(0.0f), vec3(0.0f), vec3(1.0f));
}

dict<SDNodeType, ivec2> ObjectManager::byteSizeMap = {
	{OP_MIN, ivec2(0, sizeof(vec4))},
	{OP_MAX, ivec2(0, sizeof(vec4))},
	{OP_SMIN, ivec2(4, sizeof(vec4))},
	{OP_SMAX, ivec2(4, sizeof(vec4))},

	{PRIM_SPHERE, ivec2(4, 16)},
	{PRIM_BOX, ivec2(12, 16) },

	{SPOP_TWIST, ivec2(sizeof(float) * 1, sizeof(float) * 4)},
	{SPOP_MIRROR, ivec2(sizeof(float) * 0, sizeof(float) * 4)},
	{SPOP_REPEAT, ivec2(sizeof(float) * 7, sizeof(float) * 8)},
	{SPOP_PINCH, ivec2(sizeof(float) * 1, sizeof(float) * 4)}
};

dict<SDNodeType, string> ObjectManager::defaultNameMap = {
	{PRIM_SPHERE, "Sphere"},
	{PRIM_BOX, "Box"},

	{OP_MIN, "Union"},
	{OP_SMIN, "Smooth Union"},
	{OP_MAX, "Intersection"},
	{OP_SMAX, "Smooth Intersection"},

	{SPOP_TWIST, "Twist"},
	{SPOP_MIRROR, "Mirror"},
	{SPOP_REPEAT, "Repeat"},
	{SPOP_PINCH, "Pinch"}
};

ivec2 ObjectManager::getStructSize(SDNodeType type) {
	assert(byteSizeMap.count(type) != 0);
	return byteSizeMap[type];
}

NodeCPU* ObjectManager::addNode(NodeCPU* parent, SDNodeType type, vec3 pos, vec3 euler, vec3 scale) {
	nodes.push_back(mkU<NodeCPU>(this, parent, type));
	NodeCPU* node = nodes[nodes.size() - 1].get();
	if(parent != nullptr) parent->addChild(node);

	node->setLocalPosEulerScale(pos, euler, scale);

	return node;
}

string ObjectManager::getDefaultName(SDNodeType type) {
	assert(defaultNameMap.count(type) != 0);
	return defaultNameMap[type];
}