#pragma once
#include "helperinclude.h"
#include "drawable.h"

class Quadrangle : public Drawable {
public:
	Quadrangle();
	void createVBOData() override;
};