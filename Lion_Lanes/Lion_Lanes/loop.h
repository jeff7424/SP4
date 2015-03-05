
#define TILE_SIZE 96

float bg1_y = 0;
float bg2_y = -960;

void video_loop(void)	// 3D
{
	// ====== Objects are drawn here. ====== //

	// ---- Terrain
/*	glPushMatrix();
		glScalef(0.1,0.1,0.1);
		glTranslatef(0, -100, 0);
		glColor3f(1, 1, 1);
		RenderMesh(mesh_TerrainTest);
	glPopMatrix(); */
}


/* ================================================================ 
   ================================================================ 
   ================================================================ 

   SPRITE RENDERING:

   USE:
   glTranslatef(OwnObject.GetPosition().x-(LocalPlayer.GetPosition().x-(window Width * 0.5)), OwnObject.GetPosition().y-(LocalPlayer.GetPosition().y-(window height * 0.5)), 0);


   ================================================================ 
   ================================================================ 
   ================================================================ */


int blink_timer = 1;
void flatty_loop(void)
{
/*	// Background scrolling
	bg1_y += 100/fps;
	bg2_y += 100/fps;

	if (bg1_y > 672)
		bg1_y = -960;
	if (bg2_y > 672)
		bg2_y = -960;
*/
	// ====== 2D INIT ====== //
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(0, screen_width, screen_height, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
	
	// ====== Sprites ====== //

		// ---- Terrain
		glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
				glPushMatrix();
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glBindTexture(GL_TEXTURE_2D, tex_Terrain.id);
//					glTranslatef(0, 0, 0);
					glTranslatef(-(LocalPlayer.GetPosition().x-480), -(LocalPlayer.GetPosition().y-336), 0);

					glBegin(GL_QUADS);
						glTexCoord2f(1, 1); glVertex2f(0, 0);
						glTexCoord2f(1, 0); glVertex2f(0, 960);
						glTexCoord2f(0, 0); glVertex2f(960, 960);
						glTexCoord2f(0, 1); glVertex2f(960, 0);
					glEnd();

				glPopMatrix();
			glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
				glPushMatrix();
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glBindTexture(GL_TEXTURE_2D, tex_Terrain.id);
//					glTranslatef(0, 0, 0);
					glTranslatef(-(LocalPlayer.GetPosition().x-480), -(LocalPlayer.GetPosition().y-336), 0);

					glBegin(GL_QUADS);
						glTexCoord2f(1, 1); glVertex2f(0, 0);
						glTexCoord2f(1, 0); glVertex2f(0, 960);
						glTexCoord2f(0, 0); glVertex2f(960, 960);
						glTexCoord2f(0, 1); glVertex2f(960, 0);
					glEnd();

				glPopMatrix();
			glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		// ---- Player
		glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
				glPushMatrix();
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glBindTexture(GL_TEXTURE_2D, tex_Dragon.id);
//					glTranslatef(LocalPlayer.GetPosition().x, LocalPlayer.GetPosition().y, 0);
					glTranslatef(480, 336, 0); // Cause for SPRITE RENDERING memo

					glBegin(GL_QUADS);
						glTexCoord2f(0.5 * LocalPlayer.AnimTimer + 0.5, 1); glVertex2f((-TILE_SIZE * 0.5), (-TILE_SIZE * 0.5));
						glTexCoord2f(0.5 * LocalPlayer.AnimTimer + 0.5, 0); glVertex2f((-TILE_SIZE * 0.5), (TILE_SIZE * 0.5));
						glTexCoord2f(0.5 * LocalPlayer.AnimTimer, 0); glVertex2f((TILE_SIZE * 0.5), (TILE_SIZE * 0.5));
						glTexCoord2f(0.5 * LocalPlayer.AnimTimer, 1); glVertex2f((TILE_SIZE * 0.5), (-TILE_SIZE * 0.5));
					glEnd();

				glPopMatrix();
			glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		// ---- Rocks
		for (int i = 0; i < MAXROCKS; i++)
		{
			if (obj_Rocks[i].GetExists() == true)
			{
				glEnable(GL_TEXTURE_2D);
					glEnable(GL_BLEND);
						glPushMatrix();
							glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
							glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glBindTexture(GL_TEXTURE_2D, tex_Rock.id);
//							glTranslatef(obj_Rocks[i].GetPosition().x, obj_Rocks[i].GetPosition().y, 0);
							glTranslatef(obj_Rocks[i].GetPosition().x-(LocalPlayer.GetPosition().x-480), obj_Rocks[i].GetPosition().y-(LocalPlayer.GetPosition().y-336), 0);

							glBegin(GL_QUADS);
								glTexCoord2f(0.5, 1); glVertex2f((-TILE_SIZE * 0.5), (-TILE_SIZE * 0.5));
								glTexCoord2f(0.5, 0); glVertex2f((-TILE_SIZE * 0.5), (TILE_SIZE * 0.5));
								glTexCoord2f(0, 0); glVertex2f((TILE_SIZE * 0.5), (TILE_SIZE * 0.5));
								glTexCoord2f(0, 1); glVertex2f((TILE_SIZE * 0.5), (-TILE_SIZE * 0.5));
							glEnd();

						glPopMatrix();
					glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
			}
		}

		// ---- Debug

		glColor3f( 0.0f, 1.0f, 0.0f);
		/*
		printw (5.0, 25.0, 0, "Time Elapsed: %4.2f", time_elapsed);		// Time Elapsed
		printw (5.0, 45.0, 0, "TimeSinceLastFrame: %4.2f", TimeSinceLastFrame);// Time Since Last Frame
		printw (5.0, 65.0, 0, "Framerate: %4.2f", fps);					// Framerate

		printw (5.0, 125.0, 0, "Glob_Vel_X: %4.2f", LocalPlayer.GetGlobVel().x);
		printw (5.0, 145.0, 0, "Glob_Vel_Y: %4.2f", LocalPlayer.GetGlobVel().y);
		printw (5.0, 165.0, 0, "Glob_Vel_Z: %4.2f", LocalPlayer.GetGlobVel().z);

		printw (5.0, 205.0, 0, "Mouse_X: %d", Mouse.x);
		printw (5.0, 225.0, 0, "Mouse_Y: %d", Mouse.y);

		printw (5.0, 265.0, 0, "Position_X: %4.2f", LocalPlayer.GetPosition().x);
		printw (5.0, 285.0, 0, "Position_Y: %4.2f", LocalPlayer.GetPosition().y);
		*/
		// ---- Feature

		printw (330.0, 345.0, 0, "Left: %d", rocks_left);

		if (blink_timer > 0)
		printw (570.0, 345.0, 0, "%4.2fs", time_taken*0.001);		// Time Elapsed

		// Crosshair
/*		glPushMatrix();
			glBegin (GL_LINES);
				glColor3f (0.0f, 1.0f, 0.0f);
				glVertex2f ((screen_width*0.5)-15, (screen_height*0.5));
				glVertex2f ((screen_width*0.5)+15, (screen_height*0.5));
			glEnd ();
		glPopMatrix();

		glPushMatrix();
			glBegin (GL_LINES);
				glColor3f (0.0f, 1.0f, 0.0f);
				glVertex2f ((screen_width*0.5), (screen_height*0.5)-15);
				glVertex2f ((screen_width*0.5), (screen_height*0.5)+15);
			glEnd ();
		glPopMatrix();
*/
	// ====== 2D CLOSE ====== //
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void hud_loop(void)	// 2D
{
/*	// ---- HUD
	glColor3f( 0.0f, 1.0f, 0.0f);
	printw (0.0, 0.0, 0, "Origin");		// Origin (Mostly for debug) */
}

float timer_target = 1000;
bool mapinit = true;

void logic_loop(void) // called within MyDisplay
{
	if(GetAsyncKeyState(Key_P)) exit(0);

	// ====== Framerate Measurement ====== //
	time_elapsed = glutGet(GLUT_ELAPSED_TIME);			// How long program has been running
	previousTime = currentTime;
	currentTime = time_elapsed;
	TimeSinceLastFrame = currentTime - previousTime;
	fps = 1000.0f/TimeSinceLastFrame;

	// ====== Objects' Loop functions are called here. ====== //
	LocalPlayer.Loop(camera_x, camera_y, Mouse.x, Mouse.y, fps);

	// Map Initialisation, called once
	if (mapinit == true)
	{
		for (int i = 0; i < MAXROCKS; i++)
		{
			obj_Rocks[i].Create((float)(rand() % 940 + 10), (float)(rand() % 940 + 10));

		}

		rocks_left = 25;
		mapinit = false;

			HWND Game_Window = FindWindow(NULL, "Lion Lanes");
			RECT Game_Window_Receive;
			GetWindowRect(Game_Window, &Game_Window_Receive);
			SetCursorPos(488+Game_Window_Receive.left, 367+Game_Window_Receive.top); // Values correct for Windows 8. Not my option.
	}

/*	// Spawns an ORock every 1000ms

	if (iter_obj_Rocks > MAXROCKS) iter_obj_Rocks = 0;

	if (time_elapsed > timer_target)
	{
		obj_Rocks[iter_obj_Rocks].Create();
		iter_obj_Rocks++;
		timer_target += 1000;
	} */

	for (int i = 0; i < MAXROCKS; i++)
	{
		if (obj_Rocks[i].GetExists() == true)
			obj_Rocks[i].Loop(fps, LocalPlayer.GetPosition(), rocks_left);
	}

	if (rocks_left > 0) // If there are any rocks left, don't freeze the timer
	{
		time_taken = time_elapsed;
	}
	else
	{
		blink_timer--;
		if (blink_timer < -3)
		{
			blink_timer = 10;
		}
	}
}