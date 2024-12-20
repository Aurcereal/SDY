#pragma once

#include "helperinclude.h"

struct InputBundle {
	bool w, a, s, d, q, e;
	vec2 mousePos;
	InputBundle();
};