#include "OTemplate.h"

OTemplate::OTemplate(void)
: exists(false)
, Position(0, 0, 0)
, Orientation(0, 0, 0)
{
}

OTemplate::~OTemplate(void)
{
}

// Loop Instructions
bool OTemplate::Loop(float timeDiff, Vector3D trg)
{
	if (exists == false)
		return true;

	// Update Position // Speed and movement
	Position.Set(	Position.x + Orientation.x*timeDiff, 
						Position.y + Orientation.y*timeDiff, 
						Position.z + Orientation.z*timeDiff);

	glPushMatrix();
		glTranslatef(Position.x, Position.y, Position.z);
		glRotatef( atan(Orientation.x / Orientation.z)*180.0f/3.142f , 0, 1, 0);

		// Model?
		
	glPopMatrix();

	return true;
}

// Create
void OTemplate::Create(bool m_bActivate)
{
	if (m_bActivate == true)
		exists = true;
	else
		exists = false;
}

// get status of the object. true == active, false == inactive
bool OTemplate::GetExists(void)
{
	return exists;
}

// Set the position of the object
void OTemplate::SetPosition(Vector3D NewPosition)
{
	Position.Set( NewPosition.x, NewPosition.y, NewPosition.z);
}

// Get the position of the object
Vector3D OTemplate::GetPosition(void)
{
	return Position;
}

// Set the direction of the object
void OTemplate::SetOrientation(Vector3D NewOrientation)
{
	Orientation.Set( NewOrientation.x, NewOrientation.y, NewOrientation.z);
}

Vector3D OTemplate::GetOrientation(void)
{
	return Orientation;
}