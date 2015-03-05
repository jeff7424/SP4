#pragma once
#include "vector3D.h"
#include "glut.h"
#include <stdio.h>

class CProjectile
{
public:
	CProjectile(void);
	~CProjectile(void);
private:
	// Boolean flag to indicate if this projectile is active. If not active, then do not compute/update
	bool m_bActive;
public:
	// Update the status of this projectile
	bool Update(float timeDiff, Vector3D trg);
	// Activate the projectile. true == active, false == inactive
	void SetActivate(bool m_bActivate);
	// get status of the projectile. true == active, false == inactive
	bool GetStatus(void);
	// Set Position
	void SetPosition(Vector3D theNewPosition);
	// Get Position
	Vector3D GetPosition(void);
	// Set the direction of the projectile
	void SetDirection(Vector3D theNewDirection);
private:
	// The position of the projectile
	Vector3D thePosition;
	// The direction of the projectile
	Vector3D theDirection;

	float lifetime;
};
