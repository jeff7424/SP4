
// ===== HID ===== //
// Keyboard
//bool myKeys[255];
//int KeyMods;	// Captures modifiers

// Mouse
typedef struct {
	bool LButtonUp;
	bool RButtonUp;
	bool MButtonUp;
	int x;
	int y;
} MouseStates;
MouseStates Mouse;

// ===== VIDEO ===== //
// Camera
float camera_x = 0, camera_y = 0, camera_z = 100;
// ^Modified by LocalPlayer Object's loop routine.

// ===== LOGIC ===== //

float time_elapsed = 0;
float currentTime = 0;
float previousTime = 0;
float TimeSinceLastFrame = 0;
float fps = 0;			// Container for frame rate

int room_current = 0;