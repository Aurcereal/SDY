#include "guimanager.h"
#include "mygl.h"
#include "math.h"
#include <array>
#include <iostream>

GUIManager::GUIManager(ObjectManager* manager) : objectManager(manager), selectedNode(nullptr), widgetsEnabled(true), currOperation(ImGuizmo::OPERATION::TRANSLATE) {}

bool GUIManager::drawNode(NodeCPU* node) {
	// https://kahwei.dev/2022/06/20/imgui-tree-node/
	ImGuiTreeNodeFlags flags = 
		ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	if (selectedNode == node) flags |= ImGuiTreeNodeFlags_Selected;
	if (node->isLeaf()) flags |= ImGuiTreeNodeFlags_Leaf;

	bool isOpen = ImGui::TreeNodeEx(node->name.c_str(), flags);

	bool isClicked = ImGui::IsItemClicked(); // Use right after TreeNodeEx
	if (isClicked) selectedNode = node;

	return isOpen;
}

void GUIManager::recursiveDrawTree(NodeCPU* node) {
	if (drawNode(node) && !node->isLeaf()) {
		for (NodeCPU* child : node->children) {
			recursiveDrawTree(child);
		}
	}
	ImGui::TreePop();
}

void GUIManager::drawImGuiElements(const Camera& camera) {
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar);
	drawMenuBar();
	recursiveDrawTree(objectManager->root);
	ImGui::End();

	drawGizmos(camera);
	drawObjectEditorPanel();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GUIManager::tryAddObject(SDNodeType type) {
	assert(type >= 0);
	NodeCPU* parentNode = selectedNode != nullptr ? selectedNode : objectManager->root;
	if (selectedNode != nullptr && selectedNode->type >= 0) return false;
	selectedNode = objectManager->addObject(parentNode, type, vec3(0.0f), vec3(0.0f));
	return true;
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
					tryAddObject(i);
			}
		}
		ImGui::EndMenuBar();
	}
}

void GUIManager::processInput(const InputBundle& input) {
	if (widgetsEnabled) {
		if (input.wDown)
			currOperation = ImGuizmo::OPERATION::TRANSLATE;
		if (input.rDown)
			currOperation = ImGuizmo::OPERATION::ROTATE;
	}
}

void GUIManager::drawGizmos(const Camera &camera) {
	
	// https://www.youtube.com/watch?v=Pegb5CZuibU&t=694s
	if (widgetsEnabled && selectedNode != nullptr) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());

		ImGuizmo::SetRect(0, 0, MyGL::screenDimensions.x, MyGL::screenDimensions.y);
		
		mat4 proj = camera.getProjectionMatrix();
		mat4 view = camera.getInverseTransform();

		mat4 transform = selectedNode->getWorldTransform();

		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj),
			currOperation, ImGuizmo::WORLD, glm::value_ptr(transform));

		if (ImGuizmo::IsUsing()) {
			selectedNode->setWorldTransform(transform);
		}
	}

}

void GUIManager::setWidgetsActive(bool b) {
	widgetsEnabled = b;
}

void GUIManager::drawObjectEditorPanel() {
	ImGui::Begin("Editor");

	if (selectedNode != nullptr) {
		ImGui::SeparatorText("Edit Transform");

		vec3 p, e;
		selectedNode->getLocalPosEuler(&p, &e);

		float pos[3] = { p.x, p.y, p.z };
		float euler[3] = { e.x, e.y, e.z };

		bool changed = false;
		changed |= ImGui::InputFloat3("Translate", pos);
		changed |= ImGui::InputFloat3("Euler", euler);

		if (changed)
			selectedNode->setLocalPosEuler(vec3(pos[0], pos[1], pos[2]), vec3(euler[0], euler[1], euler[2]));
	}
	else {
		ImGui::Text("No primitive selected.");
	}

	ImGui::End();
}