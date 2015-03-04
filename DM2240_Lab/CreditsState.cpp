#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "menustate.h"
#include "creditsstate.h"

CCreditsState CCreditsState::InstructionState;

void CCreditsState::Init()
{
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	if (!LoadTGA(&Background, "menu.tga"))				// Load The Font Texture
		return; //false;			
	if (!LoadTGA(&Credits, "bin/ui/credits/credits.tga"))
		return;

	for (int i = 0; i < 255; i++){
		myKeys[i] = false;
	}

	Back_Button = new Button("bin/ui/buttons/button_back.tga", 800, 600, 128, 32);
}

void CCreditsState::Cleanup()
{
	if (Back_Button != NULL)
	{
		delete Back_Button;
		Back_Button = NULL;
		free(Back_Button);
	}

	if (theCam != NULL)
	{
		delete theCam;
		theCam = NULL;
		free(theCam);
	}
}

void CCreditsState::Pause()
{
}

void CCreditsState::Resume()
{
}

void CCreditsState::HandleEvents(CGameStateManager* theGSM)
{
	/*int m_iUserChoice = -1;

	do {
	cout << "CIntroState: Choose one <0> Go to Menu State, <1> Go to Play State : ";
	cin >> m_iUserChoice;
	cin.get();

	switch (m_iUserChoice) {
	case 0:
	theGSM->ChangeState(CMenuState::Instance());
	break;
	case 1:
	theGSM->ChangeState(CPlayState::Instance());
	break;
	default:
	cout << "Invalid choice!\n";
	m_iUserChoice = -1;
	break;
	}
	} while (m_iUserChoice == -1);*/
}

void CCreditsState::Update(CGameStateManager* theGSM)
{
	static int lastTime = GetTickCount();
	int time = GetTickCount();
	float deltaTime = (time - lastTime) / 1000.f;

	if (myKeys[VK_ESCAPE] == true)
		CGameStateManager::getInstance()->ChangeState(CMenuState::Instance());
}

void CCreditsState::Draw(CGameStateManager* theGSM)
{
	// Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	Update(theGSM);

	theCam->SetHUD(true);

	RenderBackground();
	RenderCredits();

	Back_Button->Render();

	theCam->SetHUD(false);

	// Flush off any entity which is not drawn yet, so that we maintain the frame rate.
	glFlush();

	// swapping the buffers causes the rendering above to be shown
	glutSwapBuffers();
	glutPostRedisplay();
}

bool CCreditsState::LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
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

void CCreditsState::changeSize(int w, int h)
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
}

void CCreditsState::inputKey(int key, int x, int y){}

void CCreditsState::KeyboardDown(unsigned char key, int x, int y){ myKeys[key] = true; }

void CCreditsState::KeyboardUp(unsigned char key, int x, int y){ myKeys[key] = false; }

void CCreditsState::MouseMove(int x, int y)
{
	mouseInfo.lastX = (int)((float)x / w * SCREEN_WIDTH);
	mouseInfo.lastY = (int)((float)y / h * SCREEN_HEIGHT);
	Back_Button->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
}

void CCreditsState::MouseClick(int button, int state, int x, int y)
{
	if (Back_Button->GetIsHover())
	{
		CGameStateManager::getInstance()->ChangeState(CMenuState::Instance());
	}
}

void CCreditsState::RenderBackground(void)
{
	glPushMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Background.texID);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(SCREEN_WIDTH, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, SCREEN_HEIGHT);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void CCreditsState::RenderCredits(void)
{
	glPushMatrix();
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Credits.texID);
	glTranslatef(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, -0.1f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(-256.0f, -256.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(256.0f, -256.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(256.0f, 256.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(-256.0f, 256.0f);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}