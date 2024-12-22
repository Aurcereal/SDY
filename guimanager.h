#pragma once

#include "helperinclude.h"

class GUIManager {
public:
	static void initImGui(GLFWwindow*);
	static void beforeRenderImGuiCalls();
	static void drawImGuiElements();
	static void shutdownImGui();
};