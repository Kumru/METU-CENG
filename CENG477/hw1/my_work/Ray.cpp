#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3D& origin, const Vector3D& direction)
    : origin(origin), direction(direction.Normalized())
{
}

/* Takes a parameter t and returns the point accoring to t. t is the parametric variable in the ray equation o+t*d.*/
Vector3D Ray::getPoint(float t) const 
{
	return origin + direction * t;
}

/* Takes a point p and returns the parameter t according to p such that p = o+t*d. */
float Ray::gett(const Vector3D & p) const
{
	return (p.x - origin.x) / direction.x;

	// Error check
}

