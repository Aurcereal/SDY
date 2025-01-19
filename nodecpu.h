#pragma once

#include "paramcpu.h"
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

	bool transformChildren;
public:
	friend class GUIManager;

	NodeCPU(ObjectManager*, const NodeCPU* parent, SDNodeType);

	/// <summary>
	/// Updates those of children too.
	/// </summary>
	void recomputeWorldTransforms(); // private?
	const mat4& getWorldTransform() const;
	void getLocalPosEulerScale(vec3* pos, vec3* euler, vec3* scale) const;
	void setLocalTransform(mat4);
	void setLocalPos(vec3);
	void setLocalEuler(vec3);
	void setLocalScale(vec3);
	void setLocalPosEulerScale(vec3 pos, vec3 euler, vec3 scale);
	void setWorldTransform(mat4);

	//
	void addChild(NodeCPU*);

	//
	string name;

	//
	SDNodeType type;
	int gpuArrIndex;

	// rn isLeaf() == hasObject()
	inline bool isLeaf() const { return children.empty(); }
	inline bool hasObject() const { return type >= 0; }

	//
	inline void onUpdateParam() { if (type == OP_SMIN) recomputeBoundingBoxMults(); }
	void recomputeBoundingBoxMults();
	float boundingBoxMult;

	//
	ParamCPU param;
};