#ifndef _RAY_H_
#define _RAY_H_

#include "defs.h"

// Class for Ray object. You will implement the methods
class Ray
{
public:
	Vector3D origin;	// Origin of the ray
	Vector3D direction;	// Direction of the ray

	Ray();	// Constuctor
	Ray(const Vector3D& origin, const Vector3D& direction);	// Constuctor

	Vector3D getPoint(float t) const;       // Return the point along the ray at ray parameter t
	float gett(const Vector3D & p) const;   // Return the t parameter at point p along the ray

private:
	// Write any other stuff here. 
};

#endif

