#pragma once
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define uPtr std::unique_ptr
#define mkU std::make_unique

#define dict unordered_map

typedef unsigned int uint;

using namespace std;
using namespace glm;