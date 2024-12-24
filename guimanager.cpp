#include "guimanager.h"

GUIManager::GUIManager(ObjectManager* manager) : objectManager(manager), selectedNodeIndex(-1) {}

bool GUIManager::drawNode(int index) {
	// https://kahwei.dev/2022/06/20/imgui-tree-node/
	ImGuiTreeNodeFlags flags = 
		ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	if (selectedNodeIndex == index) flags |= ImGuiTreeNodeFlags_Selected;
	if (objectManager->childArray[index].empty()) flags |= ImGuiTreeNodeFlags_Leaf;

	bool isOpen = ImGui::TreeNodeEx(std::to_string(index).c_str(), flags);

	bool isClicked = ImGui::IsItemClicked(); // Use right after TreeNodeEx
	if (isClicked) selectedNodeIndex = index;

	return isOpen;
}

void GUIManager::recursiveDrawTree(int index) {
	if (drawNode(index)) {
		for (int childIndex : objectManager->childArray[index]) {
			recursiveDrawTree(childIndex);
		}
		ImGui::TreePop();
	}
}

void GUIManager::drawImGuiElements() {
	ImGui::Begin("Scene");

	recursiveDrawTree(0);

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}