#include "MiniGame.h"

using namespace std;
int random = rand() % 3 + 1;

CMiniGame::CMiniGame()
:timer(100.0)
, playerposition(Vector3(SCREEN_WIDTH*0.5,400,0))

{
	winminigame = false;
	loseminigame = false;
	srand(time(NULL));
	offsetY = -50;
	offsetY2 = -80;
	cooldown = 0.5;
	timer = 1000;
	mggold = 0;
	//LoadTGA(&texture, filename);

	//For Mini Game
	LoadTGA(&MGBackgroundTexture, "bin/textures/battlefield.tga");
	LoadTGA(&MGCharacterTexture, "bin/textures/soldier.tga");
	LoadTGA(&MGBloodTexture, "bin/textures/bloodeffect.tga");
	LoadTGA(&MGGunTexture, "bin/textures/gun.tga");

	LoadTGA(&MGWinTexture, "bin/textures/mgwin.tga");
	LoadTGA(&MGLoseTexture, "bin/textures/mglose.tga");
}


CMiniGame::~CMiniGame()
{
}

void CMiniGame::Render()
{
	//RenderCharacter();
	//RenderBackground();
}

void CMiniGame::spawnbullet(int x, int y)
{
	if (timer >= 400 && timer <= 1000)
	{
		newBullet = new Bullet(Bullet::BULLET_TYPE::GO_NORMALBULLET);
		newBullet->SetActive(true);
	newBullet->SetPos(Vector3(x, y, 0));
		newBullet->SetVel(Vector3(0, 1, 0));
		newBullet->SetSpeed(400);
		MGBList.push_back(newBullet);
		//return newBullet;
		cout << " Bullet has spawned";
	}
	else if (timer >= 0 && timer < 400)
	{
		newBullet = new Bullet(Bullet::BULLET_TYPE::GO_NORMALBULLET);
		newBullet->SetActive(true);
		newBullet->SetPos(Vector3(x, y, 0));
		newBullet->SetVel(Vector3(0, 1, 0));
		newBullet->SetSpeed(600);
		MGBList.push_back(newBullet);
		//return newBullet;
		cout << " Bullet has spawned FASTER!";
	}
	
}

bool CMiniGame::collision(int bulletx, int bullety, int playerx, int playery)
{
	//if ((bulletx - playerx)*(bulletx - playerx) + (bullety - playery)*(bullety - playery) < 100)
	if ((bulletx > playerx - 48 && bulletx < playerx + 48) && (bullety > playery - 48 && bullety < playery + 48))
		return true;
	else
		return false;
}

void CMiniGame::update(float dt)
{
	//Blood Scrolling
	offsetY += 1.0;
	if (offsetY > 500)
	{
		offsetY = -50;
	}

	//Gun scrolling
	offsetY2 += 1.0;
	if (offsetY2 > 500)
	{
		offsetY2 = 50;
	}

	if (!winminigame)
	{
		timer -= dt;
	} 

	//Bonus gold for player
	

	//Stop time if time reaches 0
	if (timer <= 0)
	{
		timer = 0;
	}

	//If player survive within time limit
	if (timer <= 0 && loseminigame == false)
	{
		winminigame = true;
		
	}

	//Set back to false so player only get bonus once
	if (winminigame == true)
	{
		mggold = 300;
	}
	else if (winminigame == false)
	{
		SetMGGold(0);
	}

	if (cooldown > 0.0)
	{
		cooldown -= dt;
		//cout << cooldown << endl;
	}


	else if (cooldown <= 0.0 && timer > 0 && loseminigame == false && winminigame == false)
	{
		int x = rand() % SCREEN_WIDTH*0.6 + SCREEN_WIDTH*0.2;
		
		spawnbullet(x, SCREEN_HEIGHT*0.050);
		
		cooldown = 0.5;
	}

	for (int i = 0; i < MGBList.size(); ++i)
	{
		Bullet *bullet = MGBList[i];
		if (bullet->GetActive() && loseminigame == false && winminigame == false)
		{
			bullet->Update(dt);
			
			if (collision(bullet->GetPos().x, bullet->GetPos().y,this->GetPos().x, this->GetPos().y) == true)
			{
				if (winminigame == false)
				{

					cout << "DIE PLAYER DIEEEE!" << endl;
					bullet->SetActive(false);
					loseminigame = true;
				}
			}
		}
	}
	
	
	// Unspawn bullet if out of range
		for (std::vector<Bullet *>::iterator it = MGBList.begin(); it != MGBList.end(); ++it)
		{
			Bullet *newbullet = *it;
			if ((newbullet->GetActive()) && (newBullet->GetPos().y > SCREEN_HEIGHT*0.95) )
			{
			
				//newBullet->Update(dt);
				newBullet->SetActive(false);
				delete newBullet;
				MGBList.erase(it);
				newBullet = NULL;
				free(newbullet);
				break;
			}

			//else
			//{
			//	delete newBullet;
			//	MGBList.erase(it);
			//	newBullet = NULL;
			//	//free(newbullet);
			//	break;
			//}
		} 

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
	glBindTexture(GL_TEXTURE_2D, MGBackgroundTexture.texID);
	//glTranslatef(0.0f, offsetY, 0.0f);
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


void CMiniGame::RenderMGBlood()
{

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, MGBloodTexture.texID);
	glTranslatef(SCREEN_WIDTH*0.6, offsetY, 1);
	glBegin(GL_QUADS);
	int height = 100 * 1.333 / 1.5;
	glTexCoord2f(0, 0); glVertex2f(0, SCREEN_HEIGHT*0.2);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH*0.2, SCREEN_HEIGHT*0.2);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH*0.2, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}

void CMiniGame::RenderMGGun()
{

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, MGGunTexture.texID);
	glTranslatef(SCREEN_WIDTH*0.2, offsetY2, 1);
	glBegin(GL_QUADS);
	int height = 100 * 1.333 / 1.5;
	glTexCoord2f(0, 0); glVertex2f(0, SCREEN_HEIGHT*0.2);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH*0.2, SCREEN_HEIGHT*0.2);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH*0.2, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	Bullet* tempbullet = NULL;
	if (tempbullet != NULL)
	{
		delete tempbullet;
		tempbullet = NULL;
		free(tempbullet);
	}

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