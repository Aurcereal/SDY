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
	if (parentNode->type >= 0) return false;
	selectedNode = objectManager->addObject(parentNode, type, vec3(0.0f), vec3(0.0f), vec3(1.0f));
	return true;
}

bool GUIManager::tryAddOperation(SDNodeType type) {
	assert(type < 0);
	NodeCPU* parentNode = selectedNode != nullptr ? selectedNode : objectManager->root;
	if (parentNode->type >= 0) return false;
	selectedNode = objectManager->addOperation(parentNode, type, vec3(0.0f), vec3(0.0f), vec3(1.0f));
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
		
		if (ImGui::BeginMenu("Add Operation")) {
			array<bool, 4> addOperationToggles;
			addOperationToggles.fill(false);

			ImGui::MenuItem("Union", nullptr, &addOperationToggles[0]);
			ImGui::MenuItem("Smooth Union", nullptr, &addOperationToggles[1]);
			ImGui::MenuItem("Intersection", nullptr, &addOperationToggles[2]);
			ImGui::MenuItem("Smooth Intersection", nullptr, &addOperationToggles[3]);
			ImGui::EndMenu();

			for (int i = 0; i < addOperationToggles.size(); i++) {
				if (addOperationToggles[i])
					tryAddOperation(-1 - i);
			}
		}

		ImGui::EndMenuBar();
	}
}

void GUIManager::processInput(const InputBundle& input) {
	if (widgetsEnabled) {
		if (input.wDown)
			currOperation = ImGuizmo::OPERATION::TRANSLATE;
		if (input.eDown)
			currOperation = ImGuizmo::OPERATION::ROTATE;
		if (input.rDown)
			currOperation = ImGuizmo::OPERATION::SCALE;
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

		vec3 p, e, s;
		selectedNode->getLocalPosEulerScale(&p, &e, &s);

		float pos[3] = { p.x, p.y, p.z };
		float euler[3] = { e.x, e.y, e.z };
		float scale[3] = { s.x, s.y, s.z };

		bool transformChanged = false;
		transformChanged |= ImGui::InputFloat3("Translate", pos);
		transformChanged |= ImGui::InputFloat3("Euler", euler);
		transformChanged |= ImGui::InputFloat3("Scale", scale);

		if (transformChanged)
			selectedNode->setLocalPosEulerScale(vec3(pos[0], pos[1], pos[2]), vec3(euler[0], euler[1], euler[2]), vec3(scale[0], scale[1], scale[2]));

		bool paramsChanged = false;

		for (InputField& field : *selectedNode->param.getInputFields()) {
			switch (field.type) {
			case InputField::INT:
				paramsChanged |= ImGui::InputInt(field.name.c_str(), (int*) field.data);
				break;
			case InputField::FLOAT:
				paramsChanged |= ImGui::InputFloat(field.name.c_str(), (float*) field.data);
				break;
			case InputField::VEC2:
				paramsChanged |= ImGui::InputFloat2(field.name.c_str(), glm::value_ptr(*((vec2*)field.data)));
				break;
			case InputField::VEC3:
				paramsChanged |= ImGui::InputFloat3(field.name.c_str(), glm::value_ptr(*((vec3*)field.data)));
				break;
			}

			if (paramsChanged)
				selectedNode->param.updateParams();
		}
	}
	else {
		ImGui::Text("No primitive selected.");
	}

	ImGui::End();
}