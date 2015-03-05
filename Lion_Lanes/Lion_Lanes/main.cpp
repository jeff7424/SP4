
// Engine Components
#include <Windows.h>
#include <math.h>
#include <stdlib.h>		//srand, rand, etc
#include <time.h>


int screen_width = 960;
int screen_height = 672;

#include "keycodes.h"
#include "freeglut.h"				// FreeGLUT Rendering System
#include "light_define.h"			// Lighting Definition
#include "loadTGA.h"				// Textures
#include "loadobj.h"				// 3D Rendering of OBJs
#include "rendermesh.h"
#include "2dtext.h"

// Main Program
#include "globalvars.h"				// Engine Global Variables
#include "obj_inst_sys.h"			// Object System - Contains Objects!
#include "loop.h"					// Main Logic Loop


// Black Box
#include "prog_init_gfx.h"
#include "prog_init_hid.h"
#include "program_init.h"			// THIS initialises the whole program in the first place!

void main (int argc, char * argv[])
{
	srand (time(NULL));

    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (screen_width, screen_height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Lion Lanes");

	// ====== Video Loop ====== //
    glutDisplayFunc (MyDisplay);			// Render

	// ====== HID Loop ====== //

	// Using getAsyncKeyState instead
//	glutKeyboardFunc(KeyboardDown);			// Keyboard Holds
//	glutKeyboardUpFunc(KeyboardUp);			// Keyboard Ups

	glutPassiveMotionFunc(MouseMove);		// Mouse Movement
	glutMotionFunc(MouseMove);				// Mouse Movement during mouse downs, holds and ups
	glutMouseFunc(MouseClick);				// for mouse click // Superseded by GetAsyncKeyState

	// ====== Logic Loop ====== //

//	glutIdleFunc(logic_loop);				// Logic Loop // Now handled within MyDisplay

    init ();
//	glutFullScreen();
    glutMainLoop ();
}





























// Too little knowledge is a dangerous thing; You don't know what more there is to learn