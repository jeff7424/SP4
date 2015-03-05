#include "ORock.h"
#include <stdlib.h>		//srand, rand, etc

ORock::ORock(void)
: exists(false)
, Position(0, -100, 0)			// ORock: Randomised X
{
}

ORock::~ORock(void)
{
}

// Loop Instructions
bool ORock::Loop(float timeDiff, Vector3D trg, int &rocks_left)
{
	if (exists == true)
	{
/*		// Update Position // Speed and movement // Unused for stationary objects
		Position.Set(	Position.x, 
						Position.y + 100/timeDiff, 
						0);
*/
		// Destroy if exit out of bounds
/*		if (Position.y > 690)
			exists = false; */
	

		// Destroy upon collision with player (Vector3D trg)
		PosDiff.x = Position.x - trg.x;
		PosDiff.y = Position.y - trg.y;
		PosDiff.z = sqrt((PosDiff.x*PosDiff.x) + (PosDiff.y*PosDiff.y));	// Z-component used as container for magnitude in this 2D proto.

		if (PosDiff.z < 48)
		{
			exists = false;
			rocks_left--;
		}
	}
	return true;
}

// Create
void ORock::Create(float x, float y)
{
	exists = true;
	Position.x = x;
	Position.y = y;
/*	Position.x = (float)(rand() % 940 + 10); // Random between 10 and 950
	Position.y = -100; */
}

// get status of the object. true == active, false == inactive
bool ORock::GetExists(void)
{
	return exists;
}

// Set the position of the object
void ORock::SetPosition(Vector3D NewPosition)
{
	Position.Set( NewPosition.x, NewPosition.y, NewPosition.z);
}

// Get the position of the object
Vector3D ORock::GetPosition(void)
{
	return Position;
}