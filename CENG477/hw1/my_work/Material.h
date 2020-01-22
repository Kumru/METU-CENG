#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "defs.h"

// Class to hold variables related to a material
class Material
{
public:
	int id;	// Material id
	int phongExp = 0;	// Phong exponent
	Vector3D ambientRef;	// Coefficients for ambient reflection
	Vector3D diffuseRef;	// Coefficients for diffuse reflection
	Vector3D specularRef;	// Coefficients for specular reflection
	Vector3D mirrorRef;		// Coefficients for mirror reflection

	Material(void);	// Constructor
	
private:
	// Write any other stuff here
};

#endif
