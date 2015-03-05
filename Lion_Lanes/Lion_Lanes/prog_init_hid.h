/*
void init_hid(void)
{
	// Keyboard Key States
	for(int i=0; i<255; i++){
		myKeys[i] = false;
	}
}

// ====== KEYBOARD ====== //
// The following functions are passed as arguments to glutKeyboardFunc and glutKeyboardUpFunc.
// The x and y callback parameters indicate the mouse location in window relative coordinates when the key was pressed.
// To check if key is held, use: if(myKeys['kbcharhere'] == true){...}
// To check if key is tapped, an additional var may be required.

void KeyboardDown(unsigned char key, int x, int y) // 'Public'
{
	KeyMods = glutGetModifiers();
	myKeys[key] = true;
}
void KeyboardUp(unsigned char key, int x, int y) // 'Public'
{
	myKeys[key] = false;
}
*/
// The x and y callback parameters indicate the mouse location in window relative coordinates.
void MouseMove (int x, int y) // 'Public'
{
	Mouse.x = x;
	Mouse.y = y;
}

// The following functions are passed as arguments to glutMouseFunc.
// The x and y callback parameters indicate the mouse location in window relative coordinates.
// To check if button is held, use: if(Mouse.LButtonUp == GLUT_DOWN){...}
// To check if key is tapped, an additional var may be required.

void MouseClick(int button, int state, int x, int y) { // 'Public'
	switch (button) {

		case GLUT_LEFT_BUTTON:
			Mouse.LButtonUp = state;
			break;

		case GLUT_RIGHT_BUTTON:
			Mouse.RButtonUp = state;
			break;

		case GLUT_MIDDLE_BUTTON:
			Mouse.MButtonUp = state;
			break;
	}
}