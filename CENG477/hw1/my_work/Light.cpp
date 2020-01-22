#include "Light.h"

/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3D & position, const Vector3D & intensity)
    : position(position), intensity(intensity)
{
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3D PointLight::computeLightContribution(const Vector3D& p)
{
	return intensity * p;
}
