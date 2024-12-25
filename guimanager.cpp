#include "guimanager.h"
#include "mygl.h"
#include <iostream>

GUIManager::GUIManager(ObjectManager* manager) : objectManager(manager), selectedNodeIndex(-1), traTest(1.0f) {}

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

void GUIManager::drawImGuiElements(const Camera& camera) {
	ImGui::Begin("Scene");
	recursiveDrawTree(0);
	ImGui::End();

	drawGizmos(camera);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::drawGizmos(const Camera &camera) {
	
	// https://www.youtube.com/watch?v=Pegb5CZuibU&t=694s
	if (widgetsEnabled) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());

		ImGuizmo::SetRect(0, 0, MyGL::screenDimensions.x, MyGL::screenDimensions.y);
		
		mat4 proj = camera.getProjectionMatrix();
		mat4 view = camera.getInverseTransform();

		//mat4 traTest = mat4(1.0f);

		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj),
			ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(traTest));
	}

}

void GUIManager::setWidgetsActive(bool b) {
	widgetsEnabled = b;
}