#include "inputbundle.h"
#include <iostream>

InputBundle::InputBundle() : mousePos(-1.0f) { }

void InputBundle::update(GLFWwindow* window) {
	wDown = glfwGetKey(window, GLFW_KEY_W) && !w;
	rDown = glfwGetKey(window, GLFW_KEY_R) && !r;
	f3Down = glfwGetKey(window, GLFW_KEY_F3) && !f3;

	w = glfwGetKey(window, GLFW_KEY_W);
	a = glfwGetKey(window, GLFW_KEY_A);
	s = glfwGetKey(window, GLFW_KEY_S);
	d = glfwGetKey(window, GLFW_KEY_D);
	q = glfwGetKey(window, GLFW_KEY_Q);
	e = glfwGetKey(window, GLFW_KEY_E);
	r = glfwGetKey(window, GLFW_KEY_R);
	f3 = glfwGetKey(window, GLFW_KEY_F3);
}