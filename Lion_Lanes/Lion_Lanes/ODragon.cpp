#include "ODragon.h"
#include "keycodes.h"

ODragon::ODragon(void)
: exists(true)
, Position(480, 336, 0.0f)
, Orientation(0, 0, 0)
, Glob_Vel(0, 0, 0)
, AnimTimer(0)
{
}

ODragon::~ODragon(void)
{
}

// Loop Instructions
bool ODragon::Loop(float &camera_x, float &camera_y, int mouse_x, int mouse_y, float timeDiff)
{
	if (exists == true)	// If this object does not 'exist', don't loop
	{

		// Update Position // Speed and movement
		Position.Set(	Position.x + (Glob_Vel.x/timeDiff),		// Both acceleration (there) and speed (here) is timeDiffed
						Position.y + (Glob_Vel.y/timeDiff), 
						Position.z + (Glob_Vel.z/timeDiff));

		// Update Position // Instant mouse. Debug only
	//	Position.Set(mouse_x*0.5, mouse_y*0.5, 0);

		// Speedlimiter
	/*	if (Glob_Vel.GetMagnitude() > 1)
		{
			Glob_Vel
		}
	*/


		// Keyboard controls. Left in for posterity.

	/*	if(GetAsyncKeyState(Key_W))
		{
			Glob_Vel.y += 1.0f/timeDiff;				// Both acceleration (here) and speed (there) is timeDiffed
		}
		if(GetAsyncKeyState(Key_S))
		{
			Glob_Vel.y -= 1.0f/timeDiff;
		}
		if(GetAsyncKeyState(Key_A))
		{
			Glob_Vel.x -= 1.0f/timeDiff;
		}
		if(GetAsyncKeyState(Key_D))
		{
			Glob_Vel.x += 1.0f/timeDiff;
		} */


		// Get the difference between current position and Go-to position, and derive speed from there

		TargetPosition.x = mouse_x;
		TargetPosition.y = mouse_y;

		Glob_Vel.x = -4 * (480 - mouse_x);
		Glob_Vel.y = -4 * (336 - mouse_y);

		// Center Mouse

		if(GetAsyncKeyState(KEY_V))
		{
			HWND Game_Window = FindWindow(NULL, "Lion Lanes");
			
			RECT Game_Window_Receive; //, Client Receive;
//			POINT ptDiff;
//			GetClientRect(Game_Window, &Client_Receive);
			GetWindowRect(Game_Window, &Game_Window_Receive);
//			ptDiff.x = (Game_Window_Receive.right - Game_Window_Receive.left) - Client_Receive.right;
//			ptDiff.y = (Game_Window_Receive.bottom - Game_Window_Receive.top) - Client_Receive.bottom;
			SetCursorPos(488+Game_Window_Receive.left, 367+Game_Window_Receive.top); // Values correct for Windows 8. Not my option.
//			SetCursorPos((480-ptDiff.x)+Game_Window_Receive.left, (336-ptDiff.y)+Game_Window_Receive.top); // Is meant to be compatible across Windows OS's.
		}
	}
	return true;
}

// Create
void ODragon::Create(bool m_bActivate)
{
	if (m_bActivate == true)
		exists = true;
	else
		exists = false;
}

// get status of object. true == active, false == inactive
bool ODragon::GetExists(void)
{
	return exists;
}

// Set position of object
void ODragon::SetPosition(Vector3D NewPosition)
{
	Position.Set( NewPosition.x, NewPosition.y, NewPosition.z);
}

// Get position of object
Vector3D ODragon::GetPosition(void)
{
	return Position;
}

// Get global velocity of object
Vector3D ODragon::GetGlobVel(void)
{
	return Glob_Vel;
}

void ODragon::SetTargetPosition(Vector3D NewPos)									// Set 'Go-to' Position
{
	TargetPosition.Set( NewPos.x, NewPos.y, NewPos.z);
}

Vector3D ODragon::GetTargetPosition(void)											// Get 'Go-to' Position
{
	return TargetPosition;
}










// "You must be level ?? to *know* this feature"