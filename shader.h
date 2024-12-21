#pragma once
#include <string>
#include <unordered_map>

#include "helperinclude.h"
#include "drawable.h"

class Shader {
protected:
	static Shader* currShader;

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

	//
	void uniformFloat(const string& name, float f);
	void uniformVec2(const string& name, vec2 v);
	void uniformVec3(const string& name, vec3 v);
	void uniformVec4(const string& name, vec4 v);
	void uniformInt(const string& name, int i);
	void uniformVec2i(const string& name, ivec2 v);
};