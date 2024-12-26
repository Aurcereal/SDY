#include "guimanager.h"
#include "mygl.h"
#include "math.h"
#include <array>

GUIManager::GUIManager(ObjectManager* manager) : objectManager(manager), selectedNodeIndex(-1), widgetsEnabled(true) {}

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
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar);
	drawMenuBar();
	recursiveDrawTree(0);
	ImGui::End();

	drawGizmos(camera);
	drawObjectEditorPanel();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::addObject(SDNodeType type) {
	assert(type >= 0);
	int parentIndex = (selectedNodeIndex != -1 && !objectManager->nodeContainsObject(selectedNodeIndex)) ?
		selectedNodeIndex : 0;

	switch (type) {
	case PRIM_SPHERE:
		objectManager->addSphere(parentIndex, vec3(0.0f), vec3(0.0f), 1.0f);
		break;
	case PRIM_BOX:
		objectManager->addBox(parentIndex, vec3(0.0f), vec3(0.0f), vec3(1.0f));
		break;
	}
}

void GUIManager::drawMenuBar() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Add Object")) {
			array<bool, 2> addObjectToggles;
			addObjectToggles.fill(false);

			ImGui::MenuItem("Sphere", nullptr, &addObjectToggles[0]);
			ImGui::MenuItem("Box", nullptr, &addObjectToggles[1]);
			ImGui::EndMenu();

			// We align the boolean array indices with the enums
			for (int i = 0; i < addObjectToggles.size(); i++) {
				if (addObjectToggles.at(i))
					addObject(i);
			}
		}
		ImGui::EndMenuBar();
	}
}

void GUIManager::drawGizmos(const Camera &camera) {
	
	// https://www.youtube.com/watch?v=Pegb5CZuibU&t=694s
	if (widgetsEnabled && selectedNodeIndex != -1 && objectManager->nodeContainsObject(selectedNodeIndex)) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());

		ImGuizmo::SetRect(0, 0, MyGL::screenDimensions.x, MyGL::screenDimensions.y);
		
		mat4 proj = camera.getProjectionMatrix();
		mat4 view = camera.getInverseTransform();

		mat4 transform;
		objectManager->getTransformationOfNode(selectedNodeIndex, nullptr, nullptr, &transform);

		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj),
			ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::WORLD, glm::value_ptr(transform));

		if (ImGuizmo::IsUsing()) {
			vec3 p, e;
			decomposeTransformation(transform, &p, &e);
			objectManager->setTranslationEulerOfNode(selectedNodeIndex, p, e);
		}
	}

}

void GUIManager::setWidgetsActive(bool b) {
	widgetsEnabled = b;
}

void GUIManager::drawObjectEditorPanel() {
	ImGui::Begin("Editor");

	if (selectedNodeIndex != -1 && objectManager->nodeContainsObject(selectedNodeIndex)) {
		ImGui::SeparatorText("Edit Transform");

		vec3 p, e;
		objectManager->getTransformationOfNode(selectedNodeIndex, &p, &e, nullptr);

		float pos[3] = { p.x, p.y, p.z };
		float euler[3] = { e.x, e.y, e.z };

		bool changed = false;
		changed |= ImGui::InputFloat3("Translate", pos);
		changed |= ImGui::InputFloat3("Euler", euler);

		if (changed)
			objectManager->setTranslationEulerOfNode(selectedNodeIndex, vec3(pos[0], pos[1], pos[2]), vec3(euler[0], euler[1], euler[2]));
	}
	else {
		ImGui::Text("No primitive selected.");
	}

	ImGui::End();
}