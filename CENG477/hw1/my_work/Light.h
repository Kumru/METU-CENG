#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "defs.h"

using namespace std;

// Class for point lights
class PointLight
{
public:	
    Vector3D position;	// Position of the point light

    PointLight(const Vector3D & position, const Vector3D & intensity);	// Constructor
    Vector3D computeLightContribution(const Vector3D& p); // Compute the contribution of light at point p

private:

    Vector3D intensity;	// Intensity of the point light
};


#endif
