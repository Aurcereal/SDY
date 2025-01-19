#pragma once

#include "helperinclude.h"
#include "sdydatadefines.glsl"
#include <vector>
#include <string>

class ObjectManager;
class NodeCPU;

class InputField {
public:
	enum InputFieldType {
		INT,
		FLOAT,
		VEC2,
		VEC3
	};

	string name;
	InputFieldType type;
	void* data;
	inline InputField(string name, InputFieldType type, void* data) : name(name), type(type), data(data) {}
};

class ParamCPU {
private:
	vector<InputField> inputFields;

	ObjectManager* objectManager;

	SDNodeType type;
	int gpuParamIndex;

	void initInputFields();

	NodeCPU* node;
public:
	ParamCPU(ObjectManager*, SDNodeType, NodeCPU*);
	
	inline vector<InputField>* getInputFields() { return &inputFields; }
	inline int getGpuIndex() { return gpuParamIndex; }
	void updateParams();
	void* getParameter(const string&); // Using string not great but whatever?
};