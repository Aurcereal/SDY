#pragma once

#include "helperinclude.h"

struct InputBundle {
	bool w, a, s, d, q, e, r, f3;
	bool wDown, rDown, f3Down;
	vec2 mousePos;
	InputBundle();
	void update(GLFWwindow*);
};