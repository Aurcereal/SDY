#include "gpuparamdata.h"
#include "objectmanager.h"

void GPUParamData::setParamData(SDNodeType type, int i, void* data) {
	switch (type) {
	case OP_MIN:
		i = i == -1 ? minOps.size() - 1 : i;
		minOps[i] = *((Min*)data);
		break;
	case OP_MAX:
		i = i == -1 ? maxOps.size() - 1 : i;
		maxOps[i] = *((Max*)data);
		break;
	case OP_SMIN:
		i = i == -1 ? sminOps.size() - 1 : i;
		sminOps[i] = *((SMin*)data);
		break;
	case OP_SMAX:
		i = i == -1 ? smaxOps.size() - 1 : i;
		smaxOps[i] = *((SMax*)data);
		break;
	case PRIM_SPHERE:
		i = i == -1 ? spheres.size() - 1 : i;
		spheres[i] = *((Sphere*)data);
		break;
	case PRIM_BOX:
		i = i == -1 ? boxes.size() - 1 : i;
		boxes[i] = *((Box*)data);
		break;
	case SPOP_TWIST:
		i = 1 == -1 ? twistSpops.size() - 1 : i;
		twistSpops[i] = *((Twist*)data);
		break;
	default: // TODO: other spops
		assert(false);
		break;
	}

	objectManager->shader->setParamData(i, type, data);
}

void GPUParamData::updateParamDataOnGPU(SDNodeType type, int i) {
	void* data = nullptr;
	switch (type) {
	case OP_MIN:
		data = &minOps[i]; break;
	case OP_MAX:
		data = &maxOps[i]; break;
	case OP_SMIN:
		data = &sminOps[i]; break;
	case OP_SMAX:
		data = &smaxOps[i]; break;
	case PRIM_SPHERE:
		data = &spheres[i]; break;
	case PRIM_BOX:
		data = &boxes[i]; break;
	case SPOP_TWIST:
		data = &twistSpops[i]; break;
		break;
	default:
		assert(false);
		break;
	}

	objectManager->shader->setParamData(i, type, data); // unoptimal who cares for now
}

int GPUParamData::pushDefaultParamData(SDNodeType type) {
	int i;
	void* data = nullptr;

	if (paramCount.count(type) == 0) paramCount[type] = 1;
	else paramCount[type]++;

	switch (type) {
	case OP_MIN:
		minOps.push_back(Min());
		i = minOps.size() - 1;
		data = &minOps[minOps.size() - 1];
		break;
	case OP_MAX:
		maxOps.push_back(Max());
		i = maxOps.size() - 1;
		data = &maxOps[maxOps.size() - 1];
		break;
	case OP_SMIN:
		sminOps.push_back(SMin());
		i = sminOps.size() - 1;
		data = &sminOps[sminOps.size() - 1];
		break;
	case OP_SMAX:
		smaxOps.push_back(SMax());
		i = smaxOps.size() - 1;
		data = &smaxOps[smaxOps.size() - 1];
		break;
	case PRIM_SPHERE:
		spheres.push_back(Sphere());
		i = spheres.size() - 1;
		data = &spheres[spheres.size() - 1];
		break;
	case PRIM_BOX:
		boxes.push_back(Box());
		i = boxes.size() - 1;
		data = &boxes[boxes.size() - 1];
		break;
	case SPOP_TWIST:
		twistSpops.push_back(Twist());
		i = twistSpops.size() - 1;
		data = &twistSpops[twistSpops.size() - 1];
		break;
	default:
		assert(false);
		break;
	}
	
	objectManager->shader->setParamData(i, type, data);
	objectManager->shader->uniformParamCount(type, i + 1);

	return i;
}

void* GPUParamData::getParamData(SDNodeType type, int i) {
	switch (type) {
	case OP_MIN:
		return &minOps[i];
		break;
	case OP_MAX:
		return &maxOps[i];
		break;
	case OP_SMIN:
		return &sminOps[i];
		break;
	case OP_SMAX:
		return &smaxOps[i];;
		break;
	case PRIM_SPHERE:
		return &spheres[i];
		break;
	case PRIM_BOX:
		return &boxes[i];
		break;
	case SPOP_TWIST:
		return &twistSpops[i];
		break;
	default:
		assert(false);
		return nullptr;
		break;
	}
}