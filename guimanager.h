#pragma once

#include "helperinclude.h"
#include "objectmanager.h"

class GUIManager {
private:
	ObjectManager* objectManager;

	int selectedNodeIndex;

	void recursiveDrawTree(int index);
	bool drawNode(int index);
public:
	GUIManager(ObjectManager*);
	void addNode(SDNodeType type); // from drop-down add menu or smth
	void drawImGuiElements();
};