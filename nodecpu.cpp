#include "nodecpu.h"

NodeCPU::NodeCPU(ObjectManager* objectManager, const NodeCPU* parent, SDNodeType type) :
	objectManager(objectManager),
	parent(parent),
	worldTransform(parent != nullptr ? parent->worldTransform : mat4()),
	invWorldTransform(parent != nullptr ? parent->invWorldTransform : mat4()),
	type(type),
	entity(),
	name(string(ObjectManager::getDefaultName(type)).append(" ").append(std::to_string(objectManager->paramData.getCount(type)))),
	param(objectManager, type, this),
	transformChildren(true)
{

	int paramInd = param.getGpuIndex();
	gpuArrIndex = type >= 0 ?
		objectManager->nodeData.pushPrimNode(parent->gpuArrIndex, paramInd, type, 0) :
		objectManager->nodeData.pushOpNode(parent != nullptr ? parent->gpuArrIndex : -1, paramInd, type);

	recomputeBoundingBoxMults();

	// Handle Special Cases
	switch (type) {
	case OP_MAX:
	case OP_SMAX:
		setVisible(false, false);
		break;
	}
}

void NodeCPU::recomputeBoundingBoxMults() {
	float localMult = type == OP_SMIN ? 1.0f + 1.4f * *((float*)param.getParameter("Smoothness")) : 1.0f; // Probably shouldn't be linear later
	boundingBoxMult = parent != nullptr ? glm::max(localMult, parent->boundingBoxMult) : localMult;

	for (auto& child : children)
		child->recomputeBoundingBoxMults();

	// BBX Mult only goes on GPU if it's an object
	if (hasObject()) {
		PrimNodeGPU* obj = objectManager->nodeData.getPrimNode(gpuArrIndex);
		obj->boundingBoxMult = boundingBoxMult;
		objectManager->nodeData.setPrimNode(gpuArrIndex, obj);
	}
}

void NodeCPU::recomputeWorldTransforms() {

	const NodeCPU* transformParent = parent;
	while (transformParent != nullptr && !transformParent->transformChildren)
		transformParent = transformParent->parent;

	if (transformParent != nullptr) {
		worldTransform = transformParent->worldTransform * entity.getTransform();
		invWorldTransform = entity.getInverseTransform() * transformParent->invWorldTransform;
	}
	else {
		worldTransform = entity.getTransform();
		invWorldTransform = entity.getInverseTransform();
	}
	for (auto& child : children) {
		child->recomputeWorldTransforms();
	}

	// Transform only goes on the GPU if it's an object
	if (hasObject()) {
		PrimNodeGPU* obj = objectManager->nodeData.getPrimNode(gpuArrIndex);
		obj->invTransform = invWorldTransform;
		objectManager->nodeData.setPrimNode(gpuArrIndex, obj);
	}
}
const mat4& NodeCPU::getWorldTransform() const {
	return worldTransform;
}
void NodeCPU::getLocalPosEulerScale(vec3* pos, vec3* euler, vec3* scale) const {
	if (pos != nullptr) *pos = entity.getPos();
	if (euler != nullptr) *euler = entity.getEuler();
	if (scale != nullptr) *scale = entity.getScale();
}
void NodeCPU::setLocalTransform(mat4 localTransform) {
	entity.setTransform(localTransform);
	recomputeWorldTransforms();
}
void NodeCPU::setLocalPos(vec3 pos) {
	entity.setPos(pos);
	recomputeWorldTransforms();
}
void NodeCPU::setLocalEuler(vec3 euler) {
	entity.setEuler(euler);
	recomputeWorldTransforms();
}
void NodeCPU::setLocalScale(vec3 scale) {
	entity.setScale(scale);
	recomputeWorldTransforms();
}
void NodeCPU::setLocalPosEulerScale(vec3 pos, vec3 euler, vec3 scale) {
	entity.setPos(pos);
	entity.setEuler(euler);
	entity.setScale(scale);
	recomputeWorldTransforms();
}
void NodeCPU::setWorldTransform(mat4 worldTransform) {
	setLocalTransform((parent != nullptr ? parent->invWorldTransform : mat4()) * worldTransform); // 1 extra set whatever
}

void NodeCPU::addChild(NodeCPU* child) {
	children.push_back(child);

	// Handle Special Cases
	switch (type) {
	case OP_MAX:
	case OP_SMAX:
		setVisible(true, false);
		break;
	}
}

void NodeCPU::setVisible(bool visible, bool updateChildren) {
	if (hasObject()) {
		PrimNodeGPU* node = objectManager->nodeData.getPrimNode(gpuArrIndex);
		node->visible = visible;
		objectManager->nodeData.setPrimNode(gpuArrIndex, node);
	}
	else {
		OpNodeGPU* node = objectManager->nodeData.getOpNode(gpuArrIndex);
		node->visible = visible;
		objectManager->nodeData.setOpNode(gpuArrIndex, node);
	}

	if (updateChildren) {
		for (auto& child : children) {
			child->setVisible(visible, true);
		}
	}
}