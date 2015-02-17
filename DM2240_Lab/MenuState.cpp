#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "PlayState.h"
#include "menustate.h"

CMenuState CMenuState::theMenuState;

void CMenuState::Init()
{
	LuaInit();

	glEnable(GL_TEXTURE_2D);
	if (!LoadTGA(&menu[0], textures[0]))				// Load The Font Texture
		return; //false;										// If Loading Failed, Return False
	if (!LoadTGA(&button[0], textures[1]))
		return;
	if (!LoadTGA(&button[1], textures[2]))
		return;

	//GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	font_style = GLUT_BITMAP_HELVETICA_18;

	for (int i = 0; i < 255; i++){
		myKeys[i] = false;
	}

	/*startButtonPos = Vector3D(0, 0, 0);
	startButtonSize = Vector3D(1, 1, 1);
	exitButtonPos = Vector3D(0, 0, 0);
	exitButtonSize = Vector3D(1, 1, 1);*/

	theCam = new Camera(Camera::LAND_CAM);
	//buttonScale = Vector3D(1, 1, 1);
}

void CMenuState::Cleanup()
{
	if (theCam != NULL)
	{
		delete theCam;
		theCam = NULL;
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
	RenderStartButton();
	RenderExitButton();

	theCam->SetHUD(false);
	// Flush off any entity which is not drawn yet, so that we maintain the frame rate.
	glFlush();

	// swapping the buffers causes the rendering above to be shown
	glutSwapBuffers();
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

void CMenuState::MouseMove(int x, int y){}

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
			if (x > startButtonPos.x - startButtonSize.x && x < startButtonPos.x + startButtonSize.x &&
				y > startButtonPos.y - startButtonSize.y && y < startButtonPos.y + startButtonSize.y)
			{
				CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());
			}
			else if (x > exitButtonPos.x - exitButtonSize.x && x < exitButtonPos.x + exitButtonSize.x &&
				y > exitButtonPos.y - exitButtonSize.y && y < exitButtonPos.y + exitButtonSize.y)
			{
				CGameStateManager::getInstance()->Cleanup();
				exit(0);
			}
		}

		break;
	}
}

void CMenuState::RenderStartButton()
{
	/*startButtonPos.x = x;
	startButtonPos.y = y;
	startButtonPos.z = z;*/
	//startButtonSize = Vector3D(125, 125, 1);

	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, button[0].texID);
	glTranslatef(startButtonPos.x, startButtonPos.y, startButtonPos.z);
	glScalef(startButtonSize.x, startButtonSize.y, startButtonSize.z);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix(); 
}

void CMenuState::RenderExitButton()
{
	/*exitButtonPos.x = x;
	exitButtonPos.y = y;
	exitButtonPos.z = z;*/
	//exitButtonSize = Vector3D(75, 75, 1);

	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, button[1].texID);
	glTranslatef(exitButtonPos.x, exitButtonPos.y, exitButtonPos.z);
	glScalef(exitButtonSize.x, exitButtonSize.y, exitButtonSize.z);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
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
	glTexCoord2f(1.0f, 1.0f); glVertex2f(800.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(800.0f, 600.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 600.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

int CMenuState::LuaInit()
{
	cout << "\nMENU INITIALIZATION\n" << endl;
	lua_State *L = lua_open();
	std::string temp;
	const char *values[15] = {
		"TEXTURE_MENU",
		"TEXTURE_START",
		"TEXTURE_EXIT",
		"STARTBUTTON_POS_X",
		"STARTBUTTON_POS_Y",
		"STARTBUTTON_POS_Z",
		"STARTBUTTON_SIZE_X",
		"STARTBUTTON_SIZE_Y",
		"STARTBUTTON_SIZE_Z",
		"EXITBUTTON_POS_X",
		"EXITBUTTON_POS_Y",
		"EXITBUTTON_POS_Z",
		"EXITBUTTON_SIZE_X",
		"EXITBUTTON_SIZE_Y",
		"EXITBUTTON_SIZE_Z"
	};

	int data[12];

	for (int i = 0; i < 12; i++)
	{
		data[i] = 0;
	}

	luaL_openlibs(L);
	if (luaL_loadfile(L, "menustate_init.lua") || lua_pcall(L, 0, 0, 0))
	{
		printf("error: %s", lua_tostring(L, -1));
	}

	for (int k = 0; k < 3; k++)
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

	for (int j = 0; j < 12; j++)
	{
		lua_getglobal(L, values[j + 3]);
		if (!lua_isnumber(L, -1))
		{
			printf("Should be number\n");
			return -1;
		}
		data[j] = lua_tonumber(L, -1);
		cout << values[j + 3] << ": " << data[j] << endl;
	}

	startButtonPos.x = data[0];
	startButtonPos.y = data[1];
	startButtonPos.z = data[2];
	startButtonSize.x = data[3];
	startButtonSize.y = data[4];
	startButtonSize.z = data[5];
	exitButtonPos.x = data[6];
	exitButtonPos.y = data[7];
	exitButtonPos.z = data[8];
	exitButtonSize.x = data[9];
	exitButtonSize.y = data[10];
	exitButtonSize.z = data[11];

	return 0;
}