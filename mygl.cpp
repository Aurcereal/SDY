#include "mygl.h"
#include "guimanager.h"

ivec2 MyGL::screenDimensions = ivec2(0);

MyGL::MyGL() : vaoHandle() {}

MyGL::~MyGL() {
	glDeleteVertexArrays(1, &vaoHandle);
}

void MyGL::initializeGL() {
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	shader.createAndCompileShaderProgram("passthrough.vert.glsl", "sdy.main.frag.glsl");
	shader.setupObjectUBOs(); //
	q.createVBOData();
}

void MyGL::render() {
	glClearColor(0.8, 0.4, 0.2, 1.0f); // Set current clear color
	glClear(GL_COLOR_BUFFER_BIT); // Clear color buffer with current clear col

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	shader.draw(q);
}