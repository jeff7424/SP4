#pragma once

#include "vector3D.h"
#include "glut.h"
#include <stdio.h>

class OTemplate
{
public:
	OTemplate(void);
	~OTemplate(void);

	bool Loop(float timeDiff, Vector3D trg);							// Loop instructions

	void Create(bool m_bActivate);										// Create Instance
	bool GetExists(void);												// Check: Object exists?
	
	void SetPosition(Vector3D theNewPosition);							// Set Position
	Vector3D GetPosition(void);											// Get Position
	void SetOrientation(Vector3D NewOrientation);						// Set Orientation (i.e. rotation)
	Vector3D GetOrientation(void);										// Get Orientation

private:
	bool exists;	// 1 - Exists, 0 - Nonexistant

	Vector3D Position;													// Object Position
	Vector3D Orientation;												// Object Orientation
};
