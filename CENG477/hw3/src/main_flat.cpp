#include "OpenGL.h"

#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <vector>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

//#define DEBUG_LIGHT

#pragma region GLOBALS

GLFWwindow* window;

GLFWmonitor* monitor;
const GLFWvidmode* mode;

int windowWidth = 1000;
int windowHeight = 1000;

int texWidth;
int texHeight;

Camera camera(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), 0, 0);

glm::vec3 lightPos;

GLfloat heightFactor = 10.0f;
GLint textureOffset = 0;

std::vector<glm::vec3> positions;

#pragma endregion

#ifdef DEBUG_LIGHT
	float verticesReal[] = {
			-5.f, -5.f, -5.f,
			 5.f, -5.f, -5.f,
			 5.f,  5.f, -5.f,
			 5.f,  5.f, -5.f,
			-5.f,  5.f, -5.f,
			-5.f, -5.f, -5.f,

			-5.f, -5.f,  5.f,
			 5.f, -5.f,  5.f,
			 5.f,  5.f,  5.f,
			 5.f,  5.f,  5.f,
			-5.f,  5.f,  5.f,
			-5.f, -5.f,  5.f,

			-5.f,  5.f,  5.f,
			-5.f,  5.f, -5.f,
			-5.f, -5.f, -5.f,
			-5.f, -5.f, -5.f,
			-5.f, -5.f,  5.f,
			-5.f,  5.f,  5.f,

			 5.f,  5.f,  5.f,
			 5.f,  5.f, -5.f,
			 5.f, -5.f, -5.f,
			 5.f, -5.f, -5.f,
			 5.f, -5.f,  5.f,
			 5.f,  5.f,  5.f,

			-5.f, -5.f, -5.f,
			 5.f, -5.f, -5.f,
			 5.f, -5.f,  5.f,
			 5.f, -5.f,  5.f,
			-5.f, -5.f,  5.f,
			-5.f, -5.f, -5.f,

			-5.f,  5.f, -5.f,
			 5.f,  5.f, -5.f,
			 5.f,  5.f,  5.f,
			 5.f,  5.f,  5.f,
			-5.f,  5.f,  5.f,
			-5.f,  5.f, -5.f
	};
	float vertices    [] = {
			-5.f, -5.f, -5.f,
			 5.f, -5.f, -5.f,
			 5.f,  5.f, -5.f,
			 5.f,  5.f, -5.f,
			-5.f,  5.f, -5.f,
			-5.f, -5.f, -5.f,

			-5.f, -5.f,  5.f,
			 5.f, -5.f,  5.f,
			 5.f,  5.f,  5.f,
			 5.f,  5.f,  5.f,
			-5.f,  5.f,  5.f,
			-5.f, -5.f,  5.f,

			-5.f,  5.f,  5.f,
			-5.f,  5.f, -5.f,
			-5.f, -5.f, -5.f,
			-5.f, -5.f, -5.f,
			-5.f, -5.f,  5.f,
			-5.f,  5.f,  5.f,

			 5.f,  5.f,  5.f,
			 5.f,  5.f, -5.f,
			 5.f, -5.f, -5.f,
			 5.f, -5.f, -5.f,
			 5.f, -5.f,  5.f,
			 5.f,  5.f,  5.f,

			-5.f, -5.f, -5.f,
			 5.f, -5.f, -5.f,
			 5.f, -5.f,  5.f,
			 5.f, -5.f,  5.f,
			-5.f, -5.f,  5.f,
			-5.f, -5.f, -5.f,

			-5.f,  5.f, -5.f,
			 5.f,  5.f, -5.f,
			 5.f,  5.f,  5.f,
			 5.f,  5.f,  5.f,
			-5.f,  5.f,  5.f,
			-5.f,  5.f, -5.f
	};

void CreateLightBox()
{
	for(int i = 0; i < 108; i+=3)
	{
		vertices[i    ] = verticesReal[i    ] + lightPos.x;
		vertices[i + 1] = verticesReal[i + 1] + lightPos.y;
		vertices[i + 2] = verticesReal[i + 2] + lightPos.z;
	}
}
#endif // DEBUG_LIGHT

void CreateVertices()
{
	int height = texHeight, width = texWidth;
	positions.reserve((height - 1) * (width - 1) * 6);
	for(int i = 0; i < width - 1; i++)
	{
		for(int j = 0; j < height - 1; j++)
		{
			positions.push_back(glm::vec3(i    , 0, j    ));
			positions.push_back(glm::vec3(i + 1, 0, j    ));
			positions.push_back(glm::vec3(i + 1, 0, j + 1));
			positions.push_back(glm::vec3(i    , 0, j    ));
			positions.push_back(glm::vec3(i + 1, 0, j + 1));
			positions.push_back(glm::vec3(i    , 0, j + 1));
		}
	}
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Fullscreen.
	if(action == GLFW_REPEAT || action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:	glfwSetWindowShouldClose(window, GLFW_TRUE);	break;
			case GLFW_KEY_R:		heightFactor += 0.5f;							break;
			case GLFW_KEY_F:		heightFactor -= 0.5f;							break;
			case GLFW_KEY_Q:		textureOffset -= 1;								break;
			case GLFW_KEY_E:		textureOffset += 1;								break;
			case GLFW_KEY_UP:		lightPos.z += 5;
#ifdef DEBUG_LIGHT
									CreateLightBox();
#endif // DEBUG_LIGHT
																					break;
			case GLFW_KEY_DOWN:		lightPos.z -= 5;
#ifdef DEBUG_LIGHT
									CreateLightBox();
#endif // DEBUG_LIGHT
																					break;
			case GLFW_KEY_RIGHT:	lightPos.x -= 5;
#ifdef DEBUG_LIGHT
									CreateLightBox();
#endif // DEBUG_LIGHT
																					break;
			case GLFW_KEY_LEFT:		lightPos.x += 5;
#ifdef DEBUG_LIGHT
									CreateLightBox();
#endif // DEBUG_LIGHT
																					break;
			case GLFW_KEY_T:		lightPos.y += 5;
#ifdef DEBUG_LIGHT
									CreateLightBox();
#endif // DEBUG_LIGHT
																					break;
			case GLFW_KEY_G:		lightPos.y -= 5;
#ifdef DEBUG_LIGHT
									CreateLightBox();
#endif // DEBUG_LIGHT
																					break;
			case GLFW_KEY_Y:		camera.speed += 0.25f;							break;
			case GLFW_KEY_H:		camera.speed -= 0.25f;							break;
			case GLFW_KEY_X:		camera.speed = 0;								break;
			case GLFW_KEY_W:		camera.pitch += 0.5; camera.Update();			break;
			case GLFW_KEY_S:		camera.pitch -= 0.5; camera.Update();			break;
			case GLFW_KEY_A:		camera.yaw -= 0.5; camera.Update();				break;
			case GLFW_KEY_D:		camera.yaw += 0.5; camera.Update();				break;
			case GLFW_KEY_I:		camera.Reset();									break;
			case GLFW_KEY_P:		glfwSetWindowMonitor(window, monitor, 0, 0,
										mode->width, mode->height, mode->refreshRate);
																					break;
			default:																break;
		}			
	}
}

int main(int argc, char*args[])
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(1000, 1000, "477-3 FLAT", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	if (glewInit() != GLEW_OK)
		std::cout << "glewInit failed!" << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	/* Print the version */
	std::cout << glGetString(GL_VERSION) << std::endl;

	Texture textureNormal(args[2]);
	Texture textureHeight(args[1]);
	textureNormal.Bind(0);
	textureHeight.Bind(1);

	texWidth = textureNormal.GetWidth();
	texHeight = textureNormal.GetHeight();

	GLuint vertexCount = 6 * (texHeight - 1) * (texWidth - 1);

	camera.SetPos(glm::vec3(texWidth / 2.f, texWidth / 10.f, -texWidth / 4.f));
	lightPos = { texWidth / 2.f, 100, texHeight / 2.f };
	
	CreateVertices();

#ifdef DEBUG_LIGHT
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
#endif // DEBUG_LIGHT

	VertexBuffer vb(&positions[0], vertexCount * 3 * sizeof(GLfloat));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	Shader shader("res/flat.shader");
	shader.Bind();

	shader.SetUniform1i("texWidth", texWidth);
	shader.SetUniform1i("texHeight", texHeight);
	shader.SetUniform1i("colorMap", 0);
	shader.SetUniform1i("heightMap", 1);

#ifdef DEBUG_LIGHT
	GLuint vaoLight;
	glGenVertexArrays(1, &vaoLight);
	glBindVertexArray(vaoLight);
	CreateLightBox();
	Shader shaderLight("res/onlyMVP.shader");
#endif // DEBUG_LIGHT

	monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0, 0, 0, 1);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef DEBUG_LIGHT
		glBindVertexArray(vao);
		shader.Bind();
#endif // DEBUG_LIGHT

		/* UPDATE */
		
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		camera.pos += camera.speed * camera.gaze;
		glm::mat4 proj = glm::perspective(45.0f, (GLfloat)width/height, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		shader.SetUniformMat4("MVP", proj * view);
		shader.SetUniformVec3("camPos", camera.pos);
		shader.SetUniformVec3("lightPos", lightPos);
		shader.SetUniform1f("heightFactor", heightFactor);
		shader.SetUniform1i("textureOffset", textureOffset);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		
		/* UPDATE */

#ifdef DEBUG_LIGHT
		glBindVertexArray(vaoLight);
		VertexBuffer vbLight(vertices, 36 * 3 * sizeof(GLfloat));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		shaderLight.Bind();
		shaderLight.SetUniformMat4("MVP", proj * view);
		glDrawArrays(GL_TRIANGLES, 0, 36);
#endif // DEBUG_LIGHT

		glfwSwapBuffers(window);
		glfwPollEvents();
		glFlush();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
