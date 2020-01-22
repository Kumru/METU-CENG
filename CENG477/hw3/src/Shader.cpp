#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: rendererID(0)
{
	ShaderProgramSource shaderSources = ParseShader(filepath);
	rendererID = CreateShader(shaderSources.VertexSource, shaderSources.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(rendererID);
}

void Shader::Bind() const
{
	glUseProgram(rendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, GLint value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, GLfloat value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformVec3(const std::string& name, glm::vec3 v)
{
	glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
}

void Shader::SetUniformVec4(const std::string& name, glm::vec4 v)
{
	glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w);
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

GLint Shader::GetUniformLocation(const std::string& name)
{
	return glGetUniformLocation(rendererID, name.c_str());
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

GLuint Shader::CompileShader(GLuint type, const std::string& source)
{
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	char error[1000];

	glGetShaderInfoLog(id, 1000, NULL, error);
	printf("Compile status: \n %s\n", error);

	return id;
}

GLuint Shader::CreateShader(const std::string vertexShader, const std::string fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}