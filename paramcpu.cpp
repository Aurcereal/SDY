#include "paramcpu.h"
#include "objectmanager.h"

ParamCPU::ParamCPU(ObjectManager* objectManager, SDNodeType type) : objectManager(objectManager), type(type) {
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
		default: {}
	}
}

void ParamCPU::updateParams() {
	objectManager->paramData.updateParamDataOnGPU(type, gpuParamIndex);
}