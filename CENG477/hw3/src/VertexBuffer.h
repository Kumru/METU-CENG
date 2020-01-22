#pragma once

#include "OpenGL.h"

class VertexBuffer
{
private:
	GLuint rendererID;
public:
	VertexBuffer(const void* data, GLuint size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};