#include "Projectile.h"

CProjectile::CProjectile(void)
: m_bActive(false)
, thePosition(0, 0, 0)
, theDirection(0, 0, 0)
, lifetime(25.0f)
{
}

CProjectile::~CProjectile(void)
{
}

// Update the status of this projectile
bool CProjectile::Update(float timeDiff, Vector3D trg)
{
	if (m_bActive == false)
		return true;

	// Update lifetime of projectile. Set to inactive if too long
	lifetime -= 0.5f*timeDiff;
	if (lifetime < 0)
	{
		m_bActive = false;
		lifetime = 25.0f;
	}

	// Update Position // Speed and movement
	thePosition.Set(	thePosition.x + theDirection.x*timeDiff, 
						thePosition.y + theDirection.y*timeDiff, 
						thePosition.z + theDirection.z*timeDiff);
//	thePosition.Set(	thePosition.x, 
//						thePosition.y + theDirection.y*timeDiff, 
//						thePosition.z);

	// Check for collision. if yes, then set to inactive
	if((thePosition.x > trg.x-1.0f) && (thePosition.x < trg.x+1.0f) && (thePosition.y > trg.y-1.0f) && (thePosition.y < trg.y+1.0f) && (thePosition.z > trg.z-1.0f) && (thePosition.z < trg.z+1.0f))
	{
		m_bActive = false;
		return false;
	}

	// Check for out of bounds. if yes, then set to inactive
	if((thePosition.x > 160.0f) || (thePosition.x < 0.0f) || (thePosition.z > 160.0f) || (thePosition.z < 0.0f))
	{
		m_bActive = false;
	}

	// Draw the projectile
	// if got collision....draw collision effects

	// else draw projectile travelling


	float size = 0.08f;
	glPushMatrix();
		glTranslatef(thePosition.x, thePosition.y, thePosition.z);
		glRotatef( atan(theDirection.x / theDirection.z)*180.0f/3.142f , 0, 1, 0);

			glBegin(GL_QUADS);
			// FRONT
			glVertex3f(-size, size, size);
			glVertex3f(size, size, size);
			glVertex3f(size, -size, size);
			glVertex3f(-size, -size, size);

			// BACK
			glVertex3f(-size, -size, -size);
			glVertex3f(size, -size, -size);
			glVertex3f(size, size, -size);
			glVertex3f(-size, size, -size);

			// LEFT
			glVertex3f(-size, size, -size);
			glVertex3f(-size, size, size);
			glVertex3f(-size, -size, size);
			glVertex3f(-size, -size, -size);

			// RIGHT
			glVertex3f(size, -size, -size);
			glVertex3f(size, -size, size);
			glVertex3f(size, size, size);
			glVertex3f(size, size, -size);

			// TOP
			glVertex3f(-size, size, -size);
			glVertex3f(size, size, -size);
			glVertex3f(size, size, size);
			glVertex3f(-size, size, size);

			// BOTTOM
			glVertex3f(-size, -size, size);
			glVertex3f(size, -size, size);
			glVertex3f(size, -size, -size);
			glVertex3f(-size, -size, -size);
			glEnd();
	glPopMatrix();

	return true;
}

// Activate the projectile. true == active, false == inactive
void CProjectile::SetActivate(bool m_bActivate)
{
	if (m_bActivate == true)
		m_bActive = true;
	else
		m_bActive = false;
}

// get status of the projectile. true == active, false == inactive
bool CProjectile::GetStatus(void)
{
	return m_bActive;
}

// Set the position of the projectile
void CProjectile::SetPosition(Vector3D theNewPosition)
{
	thePosition.Set( theNewPosition.x, theNewPosition.y, theNewPosition.z);
}

// Get the position of the projectile
Vector3D CProjectile::GetPosition(void)
{
	return thePosition;
}

// Set the direction of the projectile
void CProjectile::SetDirection(Vector3D theNewDirection)
{
	theDirection.Set( theNewDirection.x, theNewDirection.y, theNewDirection.z);
}
