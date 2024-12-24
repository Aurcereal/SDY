#include "guimanager.h"

GUIManager::GUIManager(ObjectManager* manager) : objectManager(manager) {}

void GUIManager::drawImGuiElements() {
	ImGui::Begin("Window test");
	ImGui::Text("TEXT TEST");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}