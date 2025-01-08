#pragma once

#include "helperinclude.h"
#include "sdydatadefines.glsl"
#include "eulerentity.h"
#include "objectmanager.h"
#include <vector>

class ObjectManager;

class NodeCPU {
private:
	ObjectManager* objectManager;

	EulerEntity entity;

	mat4 worldTransform;
	mat4 invWorldTransform;

	vector<NodeCPU*> children;
	const NodeCPU* parent;
public:
	friend class GUIManager;

	string name;

	NodeCPU(ObjectManager*, const NodeCPU* parent, SDNodeType);

	/// <summary>
	/// Updates those of children too.
	/// </summary>
	void recomputeWorldTransforms(); // private?
	const mat4& getWorldTransform() const;
	void getLocalPosEuler(vec3* pos, vec3* euler) const;
	void setLocalTransform(mat4);
	void setLocalPos(vec3);
	void setLocalEuler(vec3);
	void setLocalPosEuler(vec3 pos, vec3 euler);
	void setWorldTransform(mat4);

	//
	void addChild(NodeCPU*);

	//
	SDNodeType type;
	int gpuArrIndex;

	// rn bit useless since isLeaf() == hasObject() always
	inline bool isLeaf() const { return children.empty(); }
	inline bool hasObject() const { return type >= 0; }
};