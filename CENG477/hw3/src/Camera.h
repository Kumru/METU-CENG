#pragma once

#include "OpenGL.h"

class Camera
{
public:
	glm::vec3 pos  = glm::vec3(0, 0,  0);
	glm::vec3 gaze = glm::vec3(0, 0, -1);
	glm::vec3 up   = glm::vec3(0, 1,  0);
	glm::vec3 left = glm::vec3(1, 0,  0);

	GLfloat speed = 0;

	GLfloat pitch = 0;
	GLfloat yaw = -90;

	Camera() {}
	Camera(glm::vec3 gaze, glm::vec3 up, GLfloat yawAngle, GLfloat pitchAngle);

	glm::mat4 GetViewMatrix();

	void SetPos(glm::vec3 position) { pos = position; orig_pos = pos; }
	void Update();
	void Reset();

private:
	glm::vec3 orig_pos  = glm::vec3(0, 0,  0);
	glm::vec3 orig_gaze = glm::vec3(0, 0, -1);
	glm::vec3 orig_up   = glm::vec3(0, 1,  0);
	glm::vec3 orig_left = glm::vec3(1, 0,  0);

	GLfloat orig_pitch = 0;
	GLfloat orig_yaw = -90;
};