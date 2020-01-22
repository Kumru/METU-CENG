#include "Camera.h"

Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3D& pos,         // Camera position
               const Vector3D& gaze,        // Camera gaze direction
               const Vector3D& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
	:id(id), pos(pos), gaze(gaze.Normalized()), up(up), imgPlane(imgPlane)
{
	int i = -1;
	while (imageName[++i])
		this->imageName[i] = imageName[i];
	this->imageName[i] = imageName[i];

	right = this->gaze.Cross(up).Normalized();
	this->up = right.Cross(this->gaze).Normalized();

	up_left_corner = this->gaze * imgPlane.distance
		+ right * imgPlane.left
		+ this->up * imgPlane.top;

	r_l_nx = right * (imgPlane.right - imgPlane.left) / imgPlane.nx;
	t_b_ny = this->up * (imgPlane.top - imgPlane.bottom) / imgPlane.ny;
}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
	Vector3D su = r_l_nx * (col + 0.5);
	Vector3D sv = t_b_ny * (row + 0.5);
	Vector3D s_minus_e = (up_left_corner + su - sv);

	return Ray(pos, s_minus_e);
}

