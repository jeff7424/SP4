#pragma once

#include "vector3D.h"
#include "glut.h"
#include <stdio.h>

class ODragon
{
public:
	ODragon(void);
	~ODragon(void);

	// Camera variables are passed by reference, then modified as per normal within the function
	bool Loop(float &camera_x, float &camera_y, int mouse_x, int mouse_y, float timeDiff);	// Loop instructions

	void Create(bool m_bActivate);										// Create Instance
	bool GetExists(void);												// Check: Object exists?
	
	void SetPosition(Vector3D theNewPosition);							// Set Position
	Vector3D GetPosition(void);											// Get Position
	Vector3D GetGlobVel(void);											// Get Global Velocity

	int AnimTimer;

private:
	bool exists;	// 1 - Exists, 0 - Nonexistant

	Vector3D Position;													// Object Position
	Vector3D Orientation;												// Object Orientation
	Vector3D Glob_Vel;													// Object Global Velocity

	// ============================== Object-Specific ============================== //

	Vector3D TargetPosition;

	void SetTargetPosition(Vector3D NewPos);									// Set 'Go-to' Position
	Vector3D GetTargetPosition(void);											// Get 'Go-to' Position
};
