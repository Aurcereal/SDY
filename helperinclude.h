#pragma once
#include <memory>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "imguizmo/ImGuizmo.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mathconstants.h"

#define uPtr std::unique_ptr
#define mkU std::make_unique

#define dict unordered_map

typedef unsigned int uint;

using namespace std;
using namespace glm;