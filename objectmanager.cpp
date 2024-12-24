#include "objectmanager.h"

ObjectManager::ObjectManager() : sdyData(mkU<SDYData>()) {}

void ObjectManager::addSphere(SDYShader* shader, const vec4 &sphere) {
	sdyData->spheres[sdyData->sphereCount] = sphere;
	shader->setSphere(sdyData->sphereCount, &sdyData->spheres[sdyData->sphereCount]);
	sdyData->sphereCount++;
}

void ObjectManager::addOperation(SDYShader* shader, const OperationNode& n) {
	sdyData->operations[sdyData->operationCount] = n;
	shader->setOperation(sdyData->operationCount, &sdyData->operations[sdyData->operationCount]);
	sdyData->operationCount++;
	shader->uniformOperationCount(sdyData->operationCount);
}