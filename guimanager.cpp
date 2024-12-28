#include "guimanager.h"
#include "mygl.h"
#include "math.h"
#include <array>
#include <iostream>

GUIManager::GUIManager(ObjectManager* manager) : objectManager(manager), selectedNode(NodeAccessor::getNull()), widgetsEnabled(true), currOperation(ImGuizmo::OPERATION::TRANSLATE) {}

bool GUIManager::drawNode(NodeAccessor accessor) {
	// https://kahwei.dev/2022/06/20/imgui-tree-node/
	ImGuiTreeNodeFlags flags = 
		ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	if (selectedNode == accessor) flags |= ImGuiTreeNodeFlags_Selected;
	if (accessor.isLeaf()) flags |= ImGuiTreeNodeFlags_Leaf;

	bool isOpen = ImGui::TreeNodeEx(objectManager->getName(accessor).c_str(), flags);

	bool isClicked = ImGui::IsItemClicked(); // Use right after TreeNodeEx
	if (isClicked) selectedNode = accessor;

	return isOpen;
}

void GUIManager::recursiveDrawTree(NodeAccessor accessor) {
	if (drawNode(accessor) && !accessor.isLeaf()) {
		for (const NodeAccessor& childAccessor : objectManager->childArray[accessor.index]) {
			recursiveDrawTree(childAccessor);
		}
	}
	ImGui::TreePop();
}

void GUIManager::drawImGuiElements(const Camera& camera) {
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar);
	drawMenuBar();
	recursiveDrawTree(NodeAccessor::getRoot());
	ImGui::End();

	drawGizmos(camera);
	drawObjectEditorPanel();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::addObject(SDNodeType type) {
	assert(type >= 0);
	int parentIndex = (!selectedNode.isNull() && !objectManager->nodeContainsObject(selectedNode)) ?
		selectedNode.index : 0;

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
	if (widgetsEnabled && !selectedNode.isNull() && objectManager->nodeContainsObject(selectedNode)) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());

		ImGuizmo::SetRect(0, 0, MyGL::screenDimensions.x, MyGL::screenDimensions.y);
		
		mat4 proj = camera.getProjectionMatrix();
		mat4 view = camera.getInverseTransform();

		mat4 transform;
		objectManager->getTransformationOfNode(selectedNode, nullptr, nullptr, &transform);

		ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj),
			currOperation, ImGuizmo::LOCAL, glm::value_ptr(transform));

		if (ImGuizmo::IsUsing()) {
			vec3 p, e;
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(p), glm::value_ptr(e), nullptr);
			objectManager->setTranslationEulerOfNode(selectedNode, p, e);
		}
	}

}

void GUIManager::setWidgetsActive(bool b) {
	widgetsEnabled = b;
}

void GUIManager::drawObjectEditorPanel() {
	ImGui::Begin("Editor");

	if (!selectedNode.isNull() && objectManager->nodeContainsObject(selectedNode)) {
		ImGui::SeparatorText("Edit Transform");

		vec3 p, e;
		objectManager->getTransformationOfNode(selectedNode, &p, &e, nullptr);

		float pos[3] = { p.x, p.y, p.z };
		float euler[3] = { e.x, e.y, e.z };

		bool changed = false;
		changed |= ImGui::InputFloat3("Translate", pos);
		changed |= ImGui::InputFloat3("Euler", euler);

		if (changed)
			objectManager->setTranslationEulerOfNode(selectedNode, vec3(pos[0], pos[1], pos[2]), vec3(euler[0], euler[1], euler[2]));
	}
	else {
		ImGui::Text("No primitive selected.");
	}

	ImGui::End();
}