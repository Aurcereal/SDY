#include "shader.h"

#include "stringhelper.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader* Shader::currShader = nullptr;

Shader::Shader() {}

void Shader::createAndCompileShaderProgram(string vertFilePath, string fragFilePath) {

	// Read from file
	string vertSource, fragSource;
	ifstream vertShaderFile, fragShaderFile;
	try {
		stringstream vStream, fStream;

		vertShaderFile.open(vertFilePath);
		vStream << vertShaderFile.rdbuf();
		vertSource = vStream.str();
		vertShaderFile.close();

		fragShaderFile.open(fragFilePath);
		fStream << fragShaderFile.rdbuf();
		fragSource = fStream.str();
		fragShaderFile.close();
	}
	catch (std::ifstream::failure err) {
		std::cout << "FAILED TO READ SHADER SOURCE >:(" << std::endl;
	}

	// Compile
	char infoLog[512];
	int success;
	GLuint vertHandle, fragHandle;
	vertHandle = glCreateShader(GL_VERTEX_SHADER);
	fragHandle = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vertSourceC = vertSource.c_str();
	const char *fragSourceC = fragSource.c_str();

	glShaderSource(vertHandle, 1, &vertSourceC, NULL);
	glCompileShader(vertHandle);
	glGetShaderiv(vertHandle, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertHandle, 512, NULL, infoLog);
		cout << "VERT COMPILE FAIL:\n" << infoLog << endl;
	}

	glShaderSource(fragHandle, 1, &fragSourceC, NULL);
	glCompileShader(fragHandle);
	glGetShaderiv(fragHandle, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragHandle, 512, NULL, infoLog);
		cout << "FRAG COMPILE FAIL:\n" << infoLog << endl;
	}

	// Link
	programHandle = glCreateProgram();
	glAttachShader(programHandle, vertHandle);
	glAttachShader(programHandle, fragHandle);
	glLinkProgram(programHandle);
	glGetProgramiv(programHandle, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programHandle, 512, NULL, infoLog);
		cout << "FAIL TO LINK SHADER PROGRAM:\n" << infoLog << std::endl;
	}

	// Delete temp objects
	glDeleteShader(vertHandle);
	glDeleteShader(fragHandle);

	//
	parseShaderForVariables(vertSource, fragSource);
}

void Shader::parseShaderForVariables(const string &vertSource, const string &fragSource) {
	use();
	vector<string> vertLines;
	split(vertSource, "\n", &vertLines);
	for (const string& line : vertLines) {
		vector<string> sub;
		split(line, " ", &sub);
		if (sub.size() == 3 && sub[0] == "uniform") {
			string name = sub[2].substr(0, sub[2].length() - 1);
			cout << "Found uniform: " << name << endl;
			uniforms[name] = glGetUniformLocation(programHandle, name.c_str());
		}
		if (sub.size() == 3 && sub[0] == "in") {
			string name = sub[2].substr(0, sub[2].length() - 1);
			cout << "Found in variable: " << name << endl;
			attributes[name] = glGetAttribLocation(programHandle, name.c_str());
		}
	}

	vector<string> fragLines;
	split(fragSource, "\n", &fragLines);
	for (const string& line : fragLines) {
		vector<string> sub;
		split(line, " ", &sub);
		if (sub.size() == 3 && sub[0] == "uniform") {
			string name = sub[2].substr(0, sub[2].length() - 1);
			cout << "Found uniform: " << name << endl;
			uniforms[name] = glGetUniformLocation(programHandle, name.c_str());
		}
	}

}

void Shader::use() {
	if (currShader == this) return;
	currShader = this;
	glUseProgram(programHandle);
}

void Shader::draw(Drawable &d) {
	if (d.indexCount <= 0) {
		throw std::invalid_argument("Attempting to draw Drawable with indexCount <= 0");
	}

	if (attributes.count("vs_Pos") != 0) {
		int attribHandle = attributes["vs_Pos"];
		assert(d.hasBuffer(POSITION));
		d.bindBuffer(POSITION);

		glEnableVertexAttribArray(attribHandle);
		glVertexAttribPointer(attribHandle, 4, GL_FLOAT, false, 0, nullptr);
	}

	d.bindBuffer(INDEX);
	glDrawElements(d.drawMode(), d.indexCount, GL_UNSIGNED_INT, 0);
	printGLErrorLog();

	if (attributes.count("vs_Pos") != 0) glDisableVertexAttribArray(attributes["vs_Pos"]);
}

void Shader::printGLErrorLog() {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		cout << "OpenGL error " << error << endl;
	}
}

void Shader::uniformFloat(const string& name, float f) {
	assert(currShader == this);
	assert(uniforms.count(name) != 0);
	glUniform1f(uniforms[name], f);
}
void Shader::uniformVec2(const string& name, vec2 v) {
	assert(currShader == this);
	assert(uniforms.count(name) != 0);
	glUniform2f(uniforms[name], v.x, v.y);
}
void Shader::uniformVec3(const string& name, vec3 v) {
	assert(currShader == this);
	assert(uniforms.count(name) != 0);
	glUniform3f(uniforms[name], v.x, v.y, v.z);
}
void Shader::uniformVec4(const string& name, vec4 v) {
	assert(currShader == this);
	assert(uniforms.count(name) != 0);
	glUniform4f(uniforms[name], v.x, v.y, v.z, v.w);
}
void Shader::uniformInt(const string& name, int i) {
	assert(currShader == this);
	assert(uniforms.count(name) != 0);
	glUniform1i(uniforms[name], i);
}
void Shader::uniformVec2i(const string& name, ivec2 v) {
	assert(currShader == this);
	assert(uniforms.count(name) != 0);
	glUniform2i(uniforms[name], v.x, v.y);
}