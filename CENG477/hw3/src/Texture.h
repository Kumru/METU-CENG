#pragma once

#include "OpenGL.h"

#include <iostream>

class Texture
{
private:
	GLuint rendererID;
	std::string filePath;
	GLint width, height, BPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(GLuint slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
};