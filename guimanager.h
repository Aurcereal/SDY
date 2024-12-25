#pragma once

#include "helperinclude.h"
#include "objectmanager.h"
#include "camera.h"
#include "imguizmo/ImGuizmo.h"

class GUIManager {
private:
	bool widgetsEnabled;

	mat4 traTest;
	ObjectManager* objectManager;

	int selectedNodeIndex;

	void recursiveDrawTree(int index);
	bool drawNode(int index);

	void drawGizmos(const Camera&);
public:
	void setWidgetsActive(bool);

	GUIManager(ObjectManager*);
	void addNode(SDNodeType type); // from drop-down add menu or smth

	void drawImGuiElements(const Camera&);
};