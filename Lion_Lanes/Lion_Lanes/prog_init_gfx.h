void init_gfx(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);	// set background colour to black
    glEnable (GL_CULL_FACE);
	glCullFace(GL_BACK);
    glShadeModel (GL_FLAT);
	glEnable (GL_DEPTH_TEST);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (50.0, (screen_width/screen_height), 1.0, 10000.0); // GLDouble(s) fovy, aspect, zNear, zFar
	glMatrixMode (GL_MODELVIEW);

	glClearColor (0.3, 0.3, 0.3, 0.0); 
	glShadeModel (GL_SMOOTH); 

	glEnable (GL_LIGHT0); 
	glLightfv (GL_LIGHT0, GL_AMBIENT, lambient); 
	glLightfv (GL_LIGHT0, GL_DIFFUSE, ldiffuse); 
	glLightfv (GL_LIGHT0, GL_SPECULAR, lspecular); 
	glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	
	glEnable(GL_NORMALIZE);
	
	// Load models into Meshes here

//	mesh_TerrainTest = LoadObj("gfx/Terrain/MilkshapeOBJTerrain.obj");
	LoadTGA(&tex_Dragon, "gfx/sprites/redpoker.tga");
	LoadTGA(&tex_Rock, "gfx/sprites/blackpoker.tga");
	LoadTGA(&tex_Terrain, "gfx/Terrain/concrete960.tga");

}

void MyDisplay (void)
{
	logic_loop();		// Program Logic

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	//V eyex,eyey,eyez,centerx,centery,centerz,upx,upy,upz.
	gluLookAt (camera_x, camera_y, camera_z, camera_x, camera_y, camera_z-1.0, 0.0, 1.0, 0.0);

	// ================================ VIDEO LOOP ================================ //
	
	// Axes - indicates +ve direction
/*	glBegin (GL_LINES);
		// Red X
		glColor3f (1.0, 0.0, 0.0);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (100.0, 0.0, 0.0);
		// Green Y
		glColor3f (0.0, 1.0, 0.0);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (0.0, 100.0, 0.0);
		// Blue Z
		glColor3f (0.0, 0.0, 1.0);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (0.0, 0.0, 100.0);
	glEnd ();
	
	// Light source
	glLightfv (GL_LIGHT0, GL_POSITION, lposition);
	glPushMatrix();
		glTranslatef(lposition[0], lposition[1], lposition[2]);
		glColor3f(1, 1, 1);
		glutSolidSphere(4,20,20);
	glPopMatrix();
*/
//	glEnable (GL_LIGHTING);				// Apply 'shading' to subsequent 3D/2D rendering instructions



//	glEnable (GL_COLOR_MATERIAL); 
//	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 

	// =========== Video Loop =========== //
//	video_loop();						// Rendering
//	glDisable(GL_LIGHTING);				// Disable 'shading' on subsequent 3D/2D rendering instructions
//	hud_loop();							// 2D displays, follow targets
	flatty_loop();						// 2D displays

//	glFlush();							// ? Flush off any entity which is not drawn yet, so that we maintain the frame rate.
	glutSwapBuffers ();
	glutPostRedisplay ();
}