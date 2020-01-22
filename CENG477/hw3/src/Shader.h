#pragma once

#include "OpenGL.h"

#include "glm/gtc/matrix_transform.hpp"

#include <string>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	GLuint rendererID;

	GLint GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& filepath);
	GLuint CreateShader(const std::string vertexShader, const std::string fragmentShader);
	GLuint CompileShader(GLuint type, const std::string& source);

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, GLint value);
	void SetUniform1f(const std::string& name, GLfloat value);
	void SetUniformVec3(const std::string& name, glm::vec3 v);
	void SetUniformVec4(const std::string& name, glm::vec4 v);
	void SetUniformMat4(const std::string& name, glm::mat4 mat);
};