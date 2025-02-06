#pragma once

#include "helperinclude.h"
#include "objectmanager.h"
#include "camera.h"
#include "imguizmo/ImGuizmo.h"

class GUIManager {
private:
	bool widgetsEnabled;
	ImGuizmo::OPERATION currOperation;

	ObjectManager* objectManager;

	NodeCPU* selectedNode;

	void drawMenuBar();
	void recursiveDrawTree(NodeCPU*);
	bool drawNode(NodeCPU*);

	void drawGizmos(const Camera&);
	void drawObjectEditorPanel();

	//
	bool tryAddObject(SDNodeType);
	bool tryAddOperation(SDNodeType);
	bool tryAddSpaceOperation(SDNodeType);
public:
	void setWidgetsActive(bool);

	GUIManager(ObjectManager*);

	void drawImGuiElements(const Camera&);
	void processInput(const InputBundle&);
};