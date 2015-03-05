#pragma once

#include "vector3D.h"
#include "glut.h"
#include <stdio.h>

class ORock
{
public:
	ORock(void);
	~ORock(void);

	bool Loop(float timeDiff, Vector3D trg, int &rocks_left);							// Loop instructions

	void Create(float x, float y);										// Create Instance
	bool GetExists(void);												// Check: Object exists?
	
	void SetPosition(Vector3D theNewPosition);							// Set Position
	Vector3D GetPosition(void);											// Get Position

private:
	bool exists;	// 1 - Exists, 0 - Nonexistant

	Vector3D Position;													// Object Position
	Vector3D PosDiff;
};
