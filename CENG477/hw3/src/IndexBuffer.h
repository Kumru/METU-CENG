#pragma once

#include "OpenGL.h"

class IndexBuffer
{
private:
	GLuint rendererID;
	GLuint count;
public:
	IndexBuffer(const GLuint* data, GLuint count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline GLuint GetCount() const { return count; }
};