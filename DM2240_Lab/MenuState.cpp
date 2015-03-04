#include <iostream>
#include <fstream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "PlayState.h"
#include "menustate.h"
#include "SettingsState.h"
#include "CreditsState.h"
#include "InstructionState.h"
#include "GameModeState.h"
#include "MiniGame.h"

CMenuState CMenuState::theMenuState;

void CMenuState::Init()
{
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	isplaying = true;
	mouseInfo.mLButtonUp = false;
	LuaInit();
	//se = createIrrKlangDevice();
	glEnable(GL_TEXTURE_2D);
	if (!LoadTGA(&menu[0], textures[0]))				// Load The Font Texture
		return; //false;										// If Loading Failed, Return False
	if (!LoadTGA(&title, textures[6]))
		return;

	font_style = GLUT_BITMAP_HELVETICA_18;

	for (int i = 0; i < 255; i++){
		myKeys[i] = false;
	}
	theCam = new Camera(Camera::LAND_CAM);

	if (se != NULL)
	{
		se->drop();
	}
	ifstream myReadFile;
	string reading;
	myReadFile.open("save/settings.txt");
	if (myReadFile.is_open())
	{
		getline(myReadFile, reading, '\n');
		audioplay = stoi(reading);
	}
	myReadFile.close();
	se = createIrrKlangDevice();
	if (audioplay == true)
	{
		bgm.setFileName("bin/sounds/main_menu.mp3");
		bgm.playSoundThreaded();
	}
}

void CMenuState::Cleanup()
{
	if (theCam != NULL)
	{
		delete theCam;
		theCam = NULL;
	}
	if (StartButton != NULL)
	{
		delete StartButton;
		StartButton = NULL;
	}
	if (SettingsButton != NULL)
	{
		delete SettingsButton;
		SettingsButton = NULL;
	}
	if (CreditsButton != NULL)
	{
		delete CreditsButton;
		CreditsButton = NULL;
	}
	if (ExitButton != NULL)
	{
		delete ExitButton;
		ExitButton = NULL;
	}

	if (MiniGameButton != NULL)
	{
		delete MiniGameButton;
		MiniGameButton = NULL;
	}

}

void CMenuState::Pause()
{
}

void CMenuState::Resume()
{
}

void CMenuState::HandleEvents(CGameStateManager* theGSM)
{
	/*int m_iUserChoice = -1;

	do {
	cout << "CMenuState: Choose one <0> Go to Intro State, <1> Go to Play State : " ;
	cin >> m_iUserChoice;
	cin.get();

	switch (m_iUserChoice) {
	case 0:
	theGSM->ChangeState( CIntroState::Instance() );
	break;

	case 1:
	theGSM->ChangeState( CPlayState::Instance() );
	break;
	default:
	cout << "Invalid choice!\n";
	m_iUserChoice = -1;
	break;
	}
	} while (m_iUserChoice == -1);*/
}

void CMenuState::Update(CGameStateManager* theGSM)
{
	//bgm.playSound("bin/sounds/main_menu.wav");

	if (myKeys['1'] == true)
		CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());

	if (myKeys[VK_ESCAPE] == true)
		exit(0);

}

void CMenuState::Draw(CGameStateManager* theGSM)
{
	// Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glEnable(GL_DEPTH_TEST);

	Update(theGSM);
	theCam->SetHUD(true);

	RenderMenu();
	RenderTitle();
	StartButton->Render();
	MiniGameButton->Render();
	SettingsButton->Render();
	CreditsButton->Render();
	ExitButton->Render();

	theCam->SetHUD(false);
	// Flush off any entity which is not drawn yet, so that we maintain the frame rate.
	glFlush();

	// swapping the buffers causes the rendering above to be shown
	glutSwapBuffers();
	glutPostRedisplay();

}

void CMenuState::printw(float x, float y, float z, char* format, ...)
{
	va_list args;	//  Variable argument list
	int len;		//	String length
	int i;			//  Iterator
	char * text;	//	Text

	//  Initialize a variable argument list
	va_start(args, format);

	//  Return the number of characters in the string referenced the list of arguments.
	//  _vscprintf doesn't count terminating '\0' (that's why +1)
	len = _vscprintf(format, args) + 1;

	//  Allocate memory for a string of the specified size
	text = (char *)malloc(len * sizeof(char));

	//  Write formatted output using a pointer to the list of arguments
	vsprintf_s(text, len, format, args);

	//  End using variable argument list 
	va_end(args);

	//  Specify the raster position for pixel operations.
	glRasterPos3f(x, y, z);


	//  Draw the characters one by one
	for (i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(font_style, text[i]);

	//  Free the allocated memory for the string
	free(text);
}

bool CMenuState::LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
{
	GLubyte		TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type = GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

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

void CMenuState::changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)(1.0f* w / h);

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

	this->w = w;
	this->h = h;

	//float ar = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

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
	//
	//this->w = w;
	//this->h = h;
}

void CMenuState::inputKey(int key, int x, int y)
{
}

void CMenuState::KeyboardDown(unsigned char key, int x, int y){

	myKeys[key] = true;
}

void CMenuState::KeyboardUp(unsigned char key, int x, int y){

	myKeys[key] = false;
}

void CMenuState::MouseMove(int x, int y)
{
	mouseInfo.lastX = (int)((float)x / w * SCREEN_WIDTH);
	mouseInfo.lastY = (int)((float)y / h * SCREEN_HEIGHT);
	CursorOnButton(mouseInfo.lastX, mouseInfo.lastY);
}

void CMenuState::MouseClick(int button, int state, int x, int y)
{
	switch (button) {

	case GLUT_LEFT_BUTTON:
		if (state == 0)
			mouseInfo.mLButtonUp = false;
		else
			mouseInfo.mLButtonUp = true;

		mouseInfo.lastX = x;
		mouseInfo.lastY = y;
		if (mouseInfo.mLButtonUp == false)
		{
			if (StartButton->GetIsHover())
			{	
				if (audioplay == true)
					se->play2D("bin/sounds/select.wav", false);
				bgm.stop();
				CGameStateManager::getInstance()->ChangeState(CGameModeState::Instance());
			}

			else if (MiniGameButton->GetIsHover())
			{
				if (audioplay == true)
					se->play2D("bin/sounds/select.wav", false);
				bgm.stop();
				CMiniGame::Instance()->SetInGame(false);
				CGameStateManager::getInstance()->ChangeState(CMiniGame::Instance());
			}

			else if (SettingsButton->GetIsHover())
			{
				if (audioplay == true)
					se->play2D("bin/sounds/select.wav", false);
				bgm.stop();
				CGameStateManager::getInstance()->ChangeState(CSettingsState::Instance());
			}
			else if (CreditsButton->GetIsHover())
			{
				if (audioplay == true)
					se->play2D("bin/sounds/select.wav", false);
				CGameStateManager::getInstance()->ChangeState(CCreditsState::Instance());
			}
			else if (ExitButton->GetIsHover())
			{
				if (audioplay == true)
					se->play2D("bin/sounds/select.wav", false);
				CGameStateManager::getInstance()->Cleanup();
				exit(0);
			}
			
		}

		break;
	}
}

void CMenuState::RenderMenu(void)
{
	glPushMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, menu[0].texID); 
	glTranslatef(0, 0, -0.1f);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(960.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(960.0f, 672.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 672.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void CMenuState::RenderTitle(void)
{
	glPushMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, title.texID);
	glTranslatef(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, -0.1f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-256.0f, -128.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(256.0f, -128.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(256.0f, 128.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-256.0f, 128.0f);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void CMenuState::CursorOnButton(int x, int y)
{
	StartButton->SetIsHover(x, y);
	MiniGameButton->SetIsHover(x,y);
	SettingsButton->SetIsHover(x, y);
	CreditsButton->SetIsHover(x, y);
	ExitButton->SetIsHover(x, y); 
	if (audioplay == true)
	{
		if (StartButton->GetIsHover())
		{
			if (isplaying == true)
			{
				se->play2D("bin/sounds/button_hover.wav", false);
				isplaying = false;
			}
		}

		else if (MiniGameButton->GetIsHover())
		{
			if (isplaying == true)
			{
				se->play2D("bin/sounds/button_hover.wav", false);
				isplaying = false;
			}
		}

		else if (SettingsButton->GetIsHover())
		{
			if (isplaying == true)
			{
				se->play2D("bin/sounds/button_hover.wav", false);
				isplaying = false;
			}
		}
		else if (CreditsButton->GetIsHover())
		{
			if (isplaying == true)
			{
				se->play2D("bin/sounds/button_hover.wav", false);
				isplaying = false;
			}
		}
		else if (ExitButton->GetIsHover())
		{
			if (isplaying == true)
			{
				se->play2D("bin/sounds/button_hover.wav", false);
				isplaying = false;
			}
		}
		else
		{
			isplaying = true;
		}
	}
}

int CMenuState::LuaInit()
{
	cout << "\nMENU INITIALIZATION\n" << endl;
	lua_State *L = lua_open();
	std::string temp;
	const char *values[32] = {
		"TEXTURE_MENU",
		"TEXTURE_START",
		"TEXTURE_SETTINGS",
		"TEXTURE_CREDITS",
		"TEXTURE_EXIT",
		"TEXTURE_MINIGAME",
		"TEXTURE_TITLE",
		"STARTBUTTON_POS_X",
		"STARTBUTTON_POS_Y",
		"STARTBUTTON_SIZE_X",
		"STARTBUTTON_SIZE_Y",
		"SETTINGSBUTTON_POS_X",
		"SETTINGSBUTTON_POS_Y",
		"SETTINGSBUTTON_SIZE_X",
		"SETTINGSBUTTON_SIZE_Y",
		"CREDITSBUTTON_POS_X",
		"CREDITSBUTTON_POS_Y",
		"CREDITSBUTTON_SIZE_X",
		"CREDITSBUTTON_SIZE_Y",
		"EXITBUTTON_POS_X",
		"EXITBUTTON_POS_Y",
		"EXITBUTTON_SIZE_X",
		"EXITBUTTON_SIZE_Y",
		"MINIGAMEBUTTON_POS_X",
		"MINIGAMEBUTTON_POS_Y",
		"MINIGAMEBUTTON_SIZE_X",
		"MINIGAMEBUTTON_SIZE_Y",
	};

	int data[20];

	for (int i = 0; i < 20; i++)
	{
		data[i] = 0;
	}

	luaL_openlibs(L);
	if (luaL_loadfile(L, "menustate_init.lua") || lua_pcall(L, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L, -1));
	}

	for (int k = 0; k < 7; k++)
	{
		lua_getglobal(L, values[k]);
		if (!lua_isstring(L, -1))
		{
			printf("Should be a string\n");
			return -1;
		}
		temp = lua_tostring(L, -1);
		char *tempdata = new char[temp.length() + 1];
		strcpy(tempdata, temp.c_str());
		textures[k] = tempdata;
		cout << values[k] << ": " << textures[k] << endl;
	}

	for (int j = 0; j < 20; j++)
	{
		lua_getglobal(L, values[j + 7]);
		if (!lua_isnumber(L, -1))
		{
			printf("Should be number\n");
			return -1;
		}
		data[j] = lua_tonumber(L, -1);
		cout << values[j + 7] << ": " << data[j] << endl;
	}

	StartButton = new Button(textures[1], data[0], data[1], data[2], data[3]);
	SettingsButton = new Button(textures[2], data[4], data[5], data[6], data[7]);
	CreditsButton = new Button(textures[3], data[8], data[9], data[10], data[11]);
	ExitButton = new Button(textures[4], data[12], data[13], data[14], data[15]);
	MiniGameButton = new Button(textures[5], data[16], data[17], data[18], data[19]);

	return 0;
}