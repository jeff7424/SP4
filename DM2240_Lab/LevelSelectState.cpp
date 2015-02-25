#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "PlayState.h"
#include "menustate.h"
#include "LevelSelectState.h"
#include "GameModeState.h"

CLevelSelectState CLevelSelectState::levelSelectState;

void CLevelSelectState::Init()
{
	mouseInfo.mLButtonUp = false;
	LuaInit();

	glEnable(GL_TEXTURE_2D);
	if (!LoadTGA(&Map, textures[0]))
		return;
	/*if (!LoadTGA(&button[0], textures[1]))
	return;
	if (!LoadTGA(&button[1], textures[2]))
	return;*/

	font_style = GLUT_BITMAP_HELVETICA_18;

	for (int i = 0; i < 255; i++){
		myKeys[i] = false;
	}

	theCam = new Camera(Camera::LAND_CAM);
}

void CLevelSelectState::Cleanup()
{
	if (theCam != NULL)
	{
		delete theCam;
		theCam = NULL;
	}
	if (BackButton != NULL)
	{
		delete BackButton;
		BackButton = NULL;
	}
}

void CLevelSelectState::Pause()
{
}

void CLevelSelectState::Resume()
{
}

void CLevelSelectState::HandleEvents(CGameStateManager* theGSM)
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

void CLevelSelectState::Update(CGameStateManager* theGSM)
{
	CursorOnButton(mouseInfo.lastX, mouseInfo.lastY);

	if (myKeys['1'] == true)
		CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());

	if (myKeys[VK_ESCAPE] == true)
		exit(0);
}

void CLevelSelectState::Draw(CGameStateManager* theGSM)
{
	// Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	Update(theGSM);

	theCam->SetHUD(true);

	RenderMenu();

	BackButton->Render();

	theCam->SetHUD(false);
	// Flush off any entity which is not drawn yet, so that we maintain the frame rate.
	glFlush();

	// swapping the buffers causes the rendering above to be shown
	glutSwapBuffers();
	glutPostRedisplay();
}

void CLevelSelectState::printw(float x, float y, float z, char* format, ...)
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

bool CLevelSelectState::LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
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

void CLevelSelectState::changeSize(int w, int h)
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

void CLevelSelectState::inputKey(int key, int x, int y)
{
}

void CLevelSelectState::KeyboardDown(unsigned char key, int x, int y){

	myKeys[key] = true;
}

void CLevelSelectState::KeyboardUp(unsigned char key, int x, int y){

	myKeys[key] = false;
}

void CLevelSelectState::MouseMove(int x, int y)
{
	mouseInfo.lastX = x;
	mouseInfo.lastY = y;

	Button_Level1->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
	Button_Level2->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
	Button_Level3->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
	Button_Level4->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
	Button_Level5->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
	Button_Level6->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
}

void CLevelSelectState::MouseClick(int button, int state, int x, int y)
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
			if (BackButton->GetIsHover())
			{
				CGameStateManager::getInstance()->ChangeState(CMenuState::Instance());
			}
			else if (Button_Level1->GetIsHover())
			{
				CPlayState::Instance()->SetLevel(1);
				CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());
			}
			else if (Button_Level2->GetIsHover())
			{
				CPlayState::Instance()->SetLevel(2);
				CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());
			}
			else if (Button_Level3->GetIsHover())
			{
				CPlayState::Instance()->SetLevel(3);
				CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());
				
			}
			else if (Button_Level4->GetIsHover())
			{
				CPlayState::Instance()->SetLevel(4);
				CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());
				
			}
			else if (Button_Level5->GetIsHover())
			{
				CPlayState::Instance()->SetLevel(5);
				CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());
			}
			else if (Button_Level6->GetIsHover())
			{
				CPlayState::Instance()->SetLevel(6);
				CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());
			}
		}

		break;
	}
}

void CLevelSelectState::RenderMenu(void)
{
	glPushMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Map.texID);
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

	Button_Level1->Render();
	Button_Level2->Render();
	Button_Level3->Render();
	Button_Level4->Render();
	Button_Level5->Render();
	Button_Level6->Render();
}

void CLevelSelectState::CursorOnButton(int x, int y)
{
	BackButton->SetIsHover(x, y);
}

int CLevelSelectState::LuaInit()
{
	cout << "\nLEVEL SELECTION INITIALIZATION\n" << endl;
	lua_State *L = lua_open();
	std::string temp;
	const char *values[31] = {
		"TEXTURE_MAP",
		"TEXTURE_BACK",
		"TEXTURE_LVL",
		"BACKBUTTON_POS_X",
		"BACKBUTTON_POS_Y",
		"BACKBUTTON_SIZE_X",
		"BACKBUTTON_SIZE_Y",
		"LVL1_POS_X",
		"LVL1_POS_Y",
		"LVL1_SIZE_X",
		"LVL1_SIZE_Y",
		"LVL2_POS_X",
		"LVL2_POS_Y",
		"LVL2_SIZE_X",
		"LVL2_SIZE_Y",
		"LVL3_POS_X",
		"LVL3_POS_Y",
		"LVL3_SIZE_X",
		"LVL3_SIZE_Y",
		"LVL4_POS_X",
		"LVL4_POS_Y",
		"LVL4_SIZE_X",
		"LVL4_SIZE_Y",
		"LVL5_POS_X",
		"LVL5_POS_Y",
		"LVL5_SIZE_X",
		"LVL5_SIZE_Y",
		"LVL6_POS_X",
		"LVL6_POS_Y",
		"LVL6_SIZE_X",
		"LVL6_SIZE_Y",
	};

	int data[28];

	for (int i = 0; i < 28; i++)
	{
		data[i] = 0;
	}

	luaL_openlibs(L);
	if (luaL_loadfile(L, "levelselectstate_init.lua") || lua_pcall(L, 0, 0, 0))
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

	for (int j = 0; j < 28; j++)
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

	BackButton = new Button(textures[1], data[0], data[1], data[2], data[3]);
	Button_Level1 = new Button(textures[2], data[4], data[5], data[6], data[7]);
	Button_Level2 = new Button(textures[2], data[8], data[9], data[10], data[11]);
	Button_Level3 = new Button(textures[2], data[12], data[13], data[14], data[15]);
	Button_Level4 = new Button(textures[2], data[16], data[17], data[18], data[19]);
	Button_Level5 = new Button(textures[2], data[20], data[21], data[22], data[23]);
	Button_Level6 = new Button(textures[2], data[24], data[25], data[26], data[27]);

	return 0;
}