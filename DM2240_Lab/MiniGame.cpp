#include "GameState.h"
#include "GameStateManager.h"
#include "MiniGame.h"
#include "MenuState.h"
#include "PlayState.h"
#include <iostream>

using namespace std;

int random = rand() % 3 + 1;

CMiniGame CMiniGame::theMiniGame;

void CMiniGame::Init()
{
	Cam = new Camera();
	timer = 100.0f;
	playerposition = Vector3(SCREEN_WIDTH*0.5, 400, 0);
	winminigame = false;
	loseminigame = false;
	srand(time(NULL));
	offsetY = 0;
	offsetY2 = 0 - SCREEN_HEIGHT;
	cooldown = 0.2;
	timer = 3000;
	mggold = 0;
	currentscore = 0;
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

	MiniGameMusic = new AudioPlayer;
	MiniGameMusic->setFileName("bin/sounds/BGM.mp3");
	MiniGameMusic->playSoundThreaded();
	//LoadTGA(&texture, filename);

	//For Mini Game
	LoadTGA(&MGBackgroundTexture1, "bin/textures/battlefield1.tga");
	LoadTGA(&MGBackgroundTexture2, "bin/textures/battlefield2.tga");
	LoadTGA(&MGCharacterTexture, "bin/textures/soldier.tga");

	LoadTGA(&MGWinTexture, "bin/textures/mgwin.tga");
	LoadTGA(&MGLoseTexture, "bin/textures/mglose.tga");
	LoadTGA(&MGLoseTexture2, "bin/textures/mglose2.tga");
}

void CMiniGame::Cleanup()
{
	while (MGBList.size() > 0)
	{
		Bullet *bullet = MGBList.back();
		delete bullet;
		bullet = NULL;
		MGBList.pop_back();
		free(bullet);
	}


}

void CMiniGame::Pause()
{

}

void CMiniGame::Resume()
{

}

void CMiniGame::HandleEvents(CGameStateManager *theGSM)
{
}

void CMiniGame::inputKey(int key, int x, int y)
{

}

void CMiniGame::KeyboardDown(unsigned char key, int x, int y)
{

}

void CMiniGame::KeyboardUp(unsigned char key, int x, int y)
{

}

void CMiniGame::Update(CGameStateManager* theGSM)
{
	static int frame = 0;
	static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	++frame;
	int time = glutGet(GLUT_ELAPSED_TIME);
	float dt = ((time - lastTime) / 1000.f);
	lastTime = time;

	static int lastFPSTime = glutGet(GLUT_ELAPSED_TIME);
	if ((time - lastFPSTime) > 1000)
	{
		lastFPSTime = time;
		frame = 0;
	}

	//Blood Scrolling
	offsetY += 5.0;
	if (offsetY > SCREEN_HEIGHT)
	{
		offsetY = -SCREEN_HEIGHT;
	}

	//Gun scrolling
	offsetY2 += 5.0;
	if (offsetY2 > SCREEN_HEIGHT)
	{
		offsetY2 = 0 - SCREEN_HEIGHT;
	}

	if (!winminigame && ingame)
	{
		timer -= dt;
	}
	
	//Outside 
	if(!ingame && winminigame == false && loseminigame == false)
	{
		timer -= dt;
		currentscore++;
	}

	//Stop time if time reaches 0
	if (timer <= 0)
	{
		timer = 0;
	}

	//If player survive within time limit
	if (timer <= 0 && loseminigame == false && ingame)
	{
		winminigame = true;
	}

	//Set back to false so player only get bonus once
	if (winminigame == true && ingame)
	{
		mggold = 300;
	}


	if (cooldown > 0.0)
	{
		cooldown -= dt;
		//cout << cooldown << endl;
	}


	else if (cooldown <= 0.0 && loseminigame == false && winminigame == false)
	{
		//Randomnise bullet spawn position
		int x = rand() % SCREEN_WIDTH*0.6 + SCREEN_WIDTH*0.2;

		spawnbullet(x, 0);

		//If mini game load from inside main game
		if (ingame && timer > 0)
		{

			if (timer >= 2000 && timer <= 3000)
			{
				cooldown = 0.4;
			}
			else if (timer >= 1200 && timer <= 2000)
			{
				cooldown = 0.3;
			}
			else if (timer > 0 && timer < 1200)
			{
				cooldown = 0.2;
			}
		}
		//If mini game load from menu screen
		else if (!ingame)
		{
			if (currentscore > 0 && currentscore <= 1200)
			{
				cooldown = 0.4;
			}
			else if (currentscore > 1000 && currentscore <= 2500)
			{
				cooldown = 0.3;
			}
			else if (currentscore > 2500 && currentscore <= 7000)
			{
				cooldown = 0.2;
			}
			else if (currentscore > 7000)
			{
				cooldown = 0.15;
			}
		}
	}

	for (int i = 0; i < MGBList.size(); ++i)
	{
		Bullet *bullet = MGBList[i];
		if (bullet->GetActive() && loseminigame == false && winminigame == false)
		{
			bullet->Update(dt);

			if (collision(bullet->GetPos().x, bullet->GetPos().y, this->GetPos().x, this->GetPos().y) == true)
			{
				//if (winminigame == false)
				//{

					//cout << "DIE PLAYER DIEEEE!" << endl;
					bullet->SetActive(false);
					loseminigame = true;
				//}
			}
		}
	}


	// Unspawn bullet if out of range
	for (std::vector<Bullet *>::iterator it = MGBList.begin(); it != MGBList.end(); ++it)
	{
		Bullet *newbullet = *it;
		if ((newbullet->GetActive()) && (newbullet->GetPos().y > SCREEN_HEIGHT*0.95))
		{
			//newBullet->Update(dt);
			newbullet->SetActive(false);
			delete newbullet;
			MGBList.erase(it);
			newbullet = NULL;
			free(newbullet);
			break;
		}

	}

	//Check if player move out of map
	if (GetPos().x >= SCREEN_WIDTH*0.8)
	{
		SetPos(Vector3(SCREEN_WIDTH*0.8, GetPos().y, 0));
	}
	else if (GetPos().x <= SCREEN_WIDTH*0.2)
	{
		SetPos(Vector3(SCREEN_WIDTH*0.2, GetPos().y, 0));
	}


	else if (GetPos().y >= SCREEN_HEIGHT*0.9)
	{
		SetPos(Vector3(GetPos().x, SCREEN_HEIGHT*0.9, 0));
	}
	else if (GetPos().y <= SCREEN_HEIGHT*0.1)
	{
		SetPos(Vector3(GetPos().x, SCREEN_HEIGHT*0.1, 0));
	}



}

void CMiniGame::SetInGame(bool ingame)
{
	this->ingame = ingame;
}

void CMiniGame::Draw(CGameStateManager* theGSM)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	Cam->Update();

	// HUD
	Cam->SetHUD(true);

	RenderMGBackground();
	RenderBullets();
	RenderMGCharacter();

	if(!ingame)
	{
	//Render the time left on minigame screen
	char temp[100];

	sprintf_s(temp, "Current Score");
	RenderStringOnScreen(SCREEN_WIDTH*0.42, SCREEN_HEIGHT*0.1, temp);

	sprintf_s(temp, " %d", GetCurrentScore());
	RenderStringOnScreen(SCREEN_WIDTH*0.46, SCREEN_HEIGHT*0.14, temp);

	}
	//Load from outside
	else if (ingame)
	{
		char temp[24];
		sprintf_s(temp, "Time Left:  %.1f", GetTimer()*0.01);
		RenderStringOnScreen(SCREEN_WIDTH*0.42, SCREEN_HEIGHT*0.1, temp);

	}

	 if (loseminigame == true && !ingame)
	{
		RenderMGLoseScreen2();
		char temp[24];
		sprintf_s(temp, "Final Score");
		RenderStringOnScreen(SCREEN_WIDTH*0.44, SCREEN_HEIGHT*0.57, temp);

		sprintf_s(temp, " %d", GetCurrentScore());
		RenderStringOnScreen(SCREEN_WIDTH*0.46, SCREEN_HEIGHT*0.62, temp);


	}


	if (winminigame == true && ingame)
	{
		RenderMGWinScreen();
		//Render Next Level Button in mini game to continue
		//WinLose_NextLevel->Render();
	}
	else if (loseminigame == true && ingame)
	{
		RenderMGLoseScreen();
	}

	Cam->SetHUD(false);

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();

}

void CMiniGame::RenderStringOnScreen(float x, float y, const char* quote)
{
	int length = strlen(quote);
	glRasterPos2f(x, y);

	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, quote[i]);
	}
}

void CMiniGame::changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	//if (h == 0)
	//	h = 1;

	//ratio = (float)(1.0f* w / h);

	//// Reset the coordinate system before modifying
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//// Set the viewport to be the entire window
	//glViewport(0, 0, w, h);

	//// Set the correct perspective.
	//gluPerspective(45, ratio, 1, 1000);
	//glMatrixMode(GL_MODELVIEW);

	if (h == 0)
		h = 1;

	float ratio = (float)w / h;

	/*WX = w;
	WY = h;*/

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
	//gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

	//float ar = 65.0f / 46.0f;

	//int viewW = w;
	//int viewH = w / ar;
	//if (viewH > h)
	//{
	//	viewH = h;
	//	viewW = h * ar;
	//}

	//int vpX = (w - viewW) / 2;
	//int vpY = (h - viewH) / 2;

	//// Reset the coordinate system before modifying
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//// Set the viewport to be the entire window
	//glViewport(vpX, vpY, viewW, viewH);

	//// Set the correct perspective.
	//gluPerspective(45, ar, 1, 1000);
	//glMatrixMode(GL_MODELVIEW);
}

void CMiniGame::MouseClick(int button, int state, int x, int y)
{
	mouseInfo.lastX = (int)((float)x / w * SCREEN_WIDTH);
	mouseInfo.lastY = (int)((float)y / h * SCREEN_HEIGHT);

	switch (button) {

	case GLUT_LEFT_BUTTON:
		if (state == 0)
		{
			if (winminigame || loseminigame)
			{
				if (ingame)
				{
					MiniGameMusic->stop();
					CPlayState::Instance()->SetLevel(4);
					CPlayState::Instance()->LoadFromFile(false);
					CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());
					mouseInfo.mLButtonUp = false;
				}
				else
				{
					MiniGameMusic->stop();
					CGameStateManager::getInstance()->ChangeState(CMenuState::Instance());
				}
			}
		}

		else
			mouseInfo.mLButtonUp = true;
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == 0)
			mouseInfo.mRButtonUp = false;
		else
			mouseInfo.mRButtonUp = true;
		break;
	}
}

void CMiniGame::MouseMove(int x, int y)
{
	mouseInfo.lastX = (int)((float)x / w * SCREEN_WIDTH);
	mouseInfo.lastY = (int)((float)y / h * SCREEN_HEIGHT);

	SetPos(Vector3(mouseInfo.lastX, mouseInfo.lastY, 0));

}

void CMiniGame::spawnbullet(int x, int y)
{
	float i = 0, d = 0;
	i = rand() % 40 - 20; //Gives a number between -20 and +20;
	d = i / 100;

	Bullet *newBullet = new Bullet(Bullet::BULLET_TYPE::GO_MINIGAMEBULLET);
	newBullet->SetActive(true);
	newBullet->SetPos(Vector3(x, y, 0));
	newBullet->SetVel(Vector3(d, 1, 0));

	////Bullet Fly from left side of screen
	//newBullet->SetPos(Vector3(-200, 200, 0));
	//newBullet->SetVel(Vector3(1, d, 0));

	//Load from minigame
	if (ingame)
	{

		if (timer >= 2000 && timer <= 3000)
		{
			newBullet->SetSpeed(600);
		}
		else if (timer >= 500 && timer < 2000)
		{
			newBullet->SetSpeed(650);
		}
		else if (timer >= 0 && timer < 500)
		{
			newBullet->SetSpeed(800);
		}
	}
	//Load from menu minigame
	else if (!ingame)
	{

		if (currentscore > 0 && currentscore <= 2000)
		{
			newBullet->SetSpeed(500);
			
		}
		else if (currentscore > 2000 && currentscore <= 5000)
		{
			newBullet->SetSpeed(600);
		}
		else if (currentscore > 5000)
		{
			newBullet->SetSpeed(800);
		}
	}

	MGBList.push_back(newBullet);
	//return newBullet;
	cout << " Bullet has spawned";

}

bool CMiniGame::collision(int bulletx, int bullety, int playerx, int playery)
{
	//if ((bulletx - playerx)*(bulletx - playerx) + (bullety - playery)*(bullety - playery) < 100)
	if ((bulletx > playerx - 48 && bulletx < playerx + 48) && (bullety > playery - 48 && bullety < playery + 48))
		return true;
	else
		return false;
}

Vector3 CMiniGame::GetPos()
{
	return playerposition;
}

void CMiniGame::SetPos(Vector3 pos)
{
	this->playerposition = pos;
}

bool CMiniGame::LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
{
	GLubyte		TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type = GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	/*
	// NEW version for opening a file
	FILE *file = NULL;
	errno_t err;

	// Open for read (will fail if file "crt_fopen_s.c" does not exist)
	err  = fopen_s( &file, filename, "rb" );
	if( err == 0 )
	{
	cout << "The file " << filename << " was opd" << endl;
	}
	else
	{
	cout << "The file " << filename << " was UNABLE to be opened!" << endl;
	}
	*/

	FILE *file = fopen(filename, "rb");						// Open The TGA File

	if (file == NULL ||										// Does File Even Exist?
		fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||	// Does The Header Match What We Want?
		fread(header, 1, sizeof(header), file) != sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
			return false;									// Return False
		else
		{
			fclose(file);									// If Anything Failed, Close The File
			return false;									// Return False
		}
	}

	texture->width = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)

	if (texture->width <= 0 ||								// Is The Width Less Than Or Equal To Zero
		texture->height <= 0 ||								// Is The Height Less Than Or Equal To Zero
		(header[4] != 24 && header[4] != 32))					// Is The TGA 24 or 32 Bit?
	{
		fclose(file);										// If Anything Failed, Close The File
		return false;										// Return False
	}

	texture->bpp = header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel = texture->bpp / 8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize = texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData = (GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if (texture->imageData == NULL ||							// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file) != imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if (texture->imageData != NULL)						// Was Image Data Loaded
			free(texture->imageData);						// If So, Release The Image Data

		fclose(file);										// Close The File
		return false;										// Return False
	}

	for (GLuint i = 0; i<int(imageSize); i += bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = texture->imageData[i];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose(file);											// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);					// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);			// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered

	if (texture[0].bpp == 24)									// Was The TGA 24 Bits
	{
		type = GL_RGB;										// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;											// Texture Building Went Ok, Return True
}

void CMiniGame::RenderBullets(void)
{
	for (int i = 0; i < MGBList.size(); ++i)
	{
		Bullet *bullet = MGBList[i];
		if (bullet->GetActive())
		{
			bullet->Render();
		}
	}
}

void CMiniGame::RenderMGBackground()
{

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, MGBackgroundTexture1.texID);
	glTranslatef(0.0f, offsetY, 0.0f);
	glBegin(GL_QUADS);
	//int height = 100 * 1.333 / 1.5;
	glTexCoord2f(0, 0); glVertex2f(0, SCREEN_HEIGHT*1.03);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT*1.03);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();


	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, MGBackgroundTexture2.texID);
	glTranslatef(0.0f, offsetY2, 0.0f);
	glBegin(GL_QUADS);
	//int height = 100 * 1.333 / 1.5;
	glTexCoord2f(0, 0); glVertex2f(0, SCREEN_HEIGHT);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();


	glDisable(GL_TEXTURE_2D);


}

void CMiniGame::RenderMGCharacter()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, MGCharacterTexture.texID);
	glTranslatef(this->GetPos().x, this->GetPos().y, this->GetPos().z);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-48, -48);
	glTexCoord2f(0, 1); glVertex2f(-48, 48);
	glTexCoord2f(1, 1); glVertex2f(48, 48);
	glTexCoord2f(1, 0); glVertex2f(48, -48);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void CMiniGame::RenderMGWinScreen()
{
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, MGWinTexture.texID);
	glTranslatef(0, 0, 0);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, SCREEN_HEIGHT);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);


}

void CMiniGame::RenderMGLoseScreen()
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, MGLoseTexture.texID);
	glTranslatef(0, 0, 0);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, SCREEN_HEIGHT);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void CMiniGame::RenderMGLoseScreen2()
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, MGLoseTexture2.texID);
	glTranslatef(0, 0, 0);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, SCREEN_HEIGHT);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}


void CMiniGame::SetTimer(int t)
{
	//timer = t;
	this->timer= t;
}

int CMiniGame::GetTimer()
{
	return timer;
}

void CMiniGame::SetMGGold(int g)
{
	this->mggold = g;
}

int CMiniGame::GetMGGold()
{
	return mggold;
}

void CMiniGame::SetCurrentScore(int cs)
{
	this->currentscore = cs;
}

int CMiniGame::GetCurrentScore()
{
	return currentscore;
}