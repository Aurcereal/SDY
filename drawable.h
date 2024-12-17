#pragma once
#include <unordered_map>

#include "helperinclude.h"

enum BufferType {
	INDEX,
	POSITION
};

class Drawable {
private:
	dict<BufferType, GLuint> buffers;
protected:
	void generateBuffer(BufferType);
	bool hasBuffer(BufferType);
	void bindBuffer(BufferType);
	void bufferData(BufferType type, void* data, GLuint size);
	void deleteBuffer(BufferType);

	GLuint indexCount;
public:
	friend class Shader;

	Drawable();
	~Drawable();

	virtual void createVBOData() = 0;
	virtual void destroyVBOData();

	virtual GLenum drawMode();
};