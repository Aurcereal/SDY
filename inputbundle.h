#pragma once

#include "helperinclude.h"

struct InputBundle {
	bool w, a, s, d, q, e, f3;
	bool f3Down;
	vec2 mousePos;
	InputBundle();
	void update(GLFWwindow*);
};