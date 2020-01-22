#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

Camera::Camera(glm::vec3 gaze, glm::vec3 up, GLfloat yawAngle, GLfloat pitchAngle)
	:gaze(gaze), up(up), yaw(yawAngle), pitch(pitchAngle)
{
	left = glm::cross(up, gaze);

	orig_pos = pos;
	orig_gaze = gaze;
	orig_up = up;
	orig_pitch = pitch;
	orig_yaw = yaw;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(pos, pos + gaze, up);
}

void Camera::Update()
{
	gaze = glm::rotate(orig_gaze, glm::radians(-yaw), orig_up);
	gaze = glm::rotate(gaze, glm::radians(-pitch), orig_left);

	gaze = glm::normalize(gaze);
	left = glm::cross(orig_up, gaze);
	up = glm::cross(gaze, orig_left);
}

void Camera::Reset()
{
	pos = orig_pos;
	gaze = orig_gaze;
	up = orig_up;
	left = orig_left;

	speed = 0;
	pitch = orig_pitch;
	yaw = orig_yaw;
}
