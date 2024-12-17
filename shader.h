#pragma once
#include <string>
#include <unordered_map>

#include "helperinclude.h"
#include "drawable.h"

class Shader {
private:
	GLuint programHandle;


	dict<string, GLuint> uniforms;
	dict<string, GLuint> attributes;
	void parseShaderForVariables(const string &vertSource, const string &fragSource);

public:
	Shader();
	void createAndCompileShaderProgram(string vertFilePath, string fragFilePath);
	void use();
	void draw(Drawable&);

	void printGLErrorLog();
};