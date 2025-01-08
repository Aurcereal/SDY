#include "nodecpu.h"

NodeCPU::NodeCPU(ObjectManager* objectManager, const NodeCPU* parent, SDNodeType type) :
	objectManager(objectManager),
	parent(parent),
	worldTransform(parent != nullptr ? parent->worldTransform : mat4()),
	invWorldTransform(parent != nullptr ? parent->invWorldTransform : mat4()),
	type(type),
	entity(),
	name(string(ObjectManager::getDefaultName(type)).append(" ").append(std::to_string(objectManager->paramData.getCount(type))))
{
	int paramInd = objectManager->paramData.pushDefaultParamData(type);
	gpuArrIndex = type >= 0 ?
		objectManager->nodeData.pushPrimNode(parent->gpuArrIndex, paramInd, type, 0) :
		objectManager->nodeData.pushOpNode(parent != nullptr ? parent->gpuArrIndex : -1, paramInd, type);
}

void NodeCPU::recomputeWorldTransforms() {
	if (parent != nullptr) {
		worldTransform = parent->worldTransform * entity.getTransform();
		invWorldTransform = entity.getInverseTransform() * parent->invWorldTransform;
	}
	else {
		worldTransform = entity.getTransform();
		invWorldTransform = entity.getInverseTransform();
	}
	for (auto& child : children) {
		child->recomputeWorldTransforms();
	}

	// Transform only goes on the GPU if it's an object
	if (type >= 0) {
		PrimNodeGPU* obj = objectManager->nodeData.getPrimNode(gpuArrIndex);
		obj->invTransform = invWorldTransform;
		objectManager->nodeData.setPrimNode(gpuArrIndex, obj);
	}
}
const mat4& NodeCPU::getWorldTransform() const {
	return worldTransform;
}
void NodeCPU::getLocalPosEuler(vec3* pos, vec3* euler) const {
	if (pos != nullptr) *pos = entity.getPos();
	if (euler != nullptr) *euler = entity.getEuler();
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
void NodeCPU::setLocalPosEuler(vec3 pos, vec3 euler) {
	entity.setPos(pos);
	entity.setEuler(euler);
	recomputeWorldTransforms();
}
void NodeCPU::setWorldTransform(mat4 worldTransform) {
	setLocalTransform((parent != nullptr ? parent->invWorldTransform : mat4()) * worldTransform); // 1 extra set whatever
}

void NodeCPU::addChild(NodeCPU* child) {
	children.push_back(child);
}