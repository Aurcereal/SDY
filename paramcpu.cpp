#include "paramcpu.h"
#include "nodecpu.h"
#include "objectmanager.h"

ParamCPU::ParamCPU(ObjectManager* objectManager, SDNodeType type, NodeCPU* node) : objectManager(objectManager), type(type), node(node) {
	gpuParamIndex = objectManager->paramData.pushDefaultParamData(type);
	initInputFields();
}

void ParamCPU::initInputFields() {
	void* data = objectManager->paramData.getParamData(type, gpuParamIndex);
	switch (type) {
		case PRIM_SPHERE: {
			Sphere* sphere = (Sphere*)data;
			inputFields.push_back(InputField("Radius", InputField::FLOAT, &sphere->r));
			break;
		}
		case PRIM_BOX: {
			Box* box = (Box*)data;
			inputFields.push_back(InputField("Dimensions", InputField::VEC3, &box->dim));
			break;
		}
		case OP_SMIN: {
			SMin* smin = (SMin*)data;
			inputFields.push_back(InputField("Smoothness", InputField::FLOAT, &smin->smoothness));
			break;
		}
		case OP_SMAX: {
			SMax* smax = (SMax*)data;
			inputFields.push_back(InputField("Smoothness", InputField::FLOAT, &smax->smoothness));
			break;
		}
	}
}

void ParamCPU::updateParams() {
	objectManager->paramData.updateParamDataOnGPU(type, gpuParamIndex);
	node->onUpdateParam();
}

void* ParamCPU::getParameter(const string& name) {
	for (const InputField& field : inputFields) {
		if (field.name == name) 
			return field.data;
	}
	return nullptr;
}