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

	NodeAccessor selectedNode;

	void drawMenuBar();
	void recursiveDrawTree(NodeAccessor);
	bool drawNode(NodeAccessor);

	void drawGizmos(const Camera&);
	void drawObjectEditorPanel();

	//
	void addObject(SDNodeType);
public:
	void setWidgetsActive(bool);

	GUIManager(ObjectManager*);

	void drawImGuiElements(const Camera&);
	void processInput(const InputBundle&);
};