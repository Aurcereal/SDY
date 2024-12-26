#pragma once

#include "helperinclude.h"
#include "objectmanager.h"
#include "camera.h"
#include "imguizmo/ImGuizmo.h"

class GUIManager {
private:
	bool widgetsEnabled;

	ObjectManager* objectManager;

	int selectedNodeIndex;

	void drawMenuBar();
	void recursiveDrawTree(int index);
	bool drawNode(int index);

	void drawGizmos(const Camera&);
	void drawObjectEditorPanel();

	//
	void addObject(SDNodeType);
public:
	void setWidgetsActive(bool);

	GUIManager(ObjectManager*);

	void drawImGuiElements(const Camera&);
};