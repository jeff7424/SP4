#include "EnemyAtt.h"
#include <GL/glut.h>

#define TILE_SIZE 96

Enemy::Enemy(ENEMY_TYPE type)
: type(type)
, buff(0)
, state(ENEMY_LANE)
, speed(0)
, offsetX(0)
, offsetY(2)
, offsetY2(0)
, tilesTravelled(0)
, called (false)
, currentTile(10)	//init position tile (softcode)
{
	srand (time(NULL));

	//heroAnimationCounter = 0;
	pattern = 0;

	if(type == Enemy::ENEMY_1)
	{
		LoadTGA(&CreepTexture[0], "bin/textures/enemy_1.tga");
		LoadTGA(&CreepTexture[1], "bin/textures/enemy_1_attack.tga");
	}
	else if (type == Enemy::ENEMY_2)
	{
		LoadTGA(&CreepTexture[0], "bin/textures/enemy_2.tga");
		LoadTGA(&CreepTexture[1], "bin/textures/enemy_2_attack.tga");
	}
	else if (type == Enemy::ENEMY_3)
	{
		LoadTGA(&CreepTexture[0], "bin/textures/enemy_3.tga");
		LoadTGA(&CreepTexture[1], "bin/textures/enemy_3_attack.tga");
	}
	else if (type == Enemy::ENEMY_4)
	{
		LoadTGA(&CreepTexture[0], "bin/textures/enemy_4.tga");
		LoadTGA(&CreepTexture[1], "bin/textures/enemy_4_attack.tga");
	}
	else if(type == Enemy::ENEMY_5)
	{
		LoadTGA(&CreepTexture[0], "bin/textures/tank.tga");
		LoadTGA(&CreepTexture[1], "bin/textures/tank.tga");
	}
}

Enemy::~Enemy()
{

}

void Enemy::DrawEnemy(int heroAnimationCounter)
{
	//int time = glutGet(GLUT_ELAPSED_TIME);
	//static int ctime = glutGet(GLUT_ELAPSED_TIME);

	//if (time - ctime > 200) // the more it is the slower it becomes
	//{
	//	heroAnimationCounter--;
	//	if (heroAnimationCounter == 0)
	//	{
	//		heroAnimationCounter = 6;
	//	}
	//	ctime = time;
	//}

	DrawHealthBar();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(!GetFire())
	{glBindTexture(GL_TEXTURE_2D, CreepTexture[0].texID);}
	else
	{glBindTexture(GL_TEXTURE_2D, CreepTexture[1].texID);}

	glTranslatef(GetPos().x, GetPos().y, 0);

	if(type == ENEMY_5)
	{
		glBegin(GL_QUADS);
		glTexCoord2f(0,1);
		glVertex2f(0, -TILE_SIZE * 1.5);
		glTexCoord2f(0,0);
		glVertex2f(0, TILE_SIZE * 1.5);
		glTexCoord2f(1,0);
		glVertex2f(TILE_SIZE * 3.5, TILE_SIZE * 1.5);
		glTexCoord2f(1,1);
		glVertex2f(TILE_SIZE * 3.5, -TILE_SIZE * 1.5);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		glTexCoord2f(0.16667 * heroAnimationCounter, 1);
		glVertex2f(-TILE_SIZE / 3, -TILE_SIZE / 3);
		glTexCoord2f(0.16667 * heroAnimationCounter, 0);
		glVertex2f(-TILE_SIZE / 3, TILE_SIZE / 3);
		glTexCoord2f(0.16667 * heroAnimationCounter + 0.16667, 0);
		glVertex2f(TILE_SIZE / 3, TILE_SIZE / 3);
		glTexCoord2f(0.16667 * heroAnimationCounter + 0.16667, 1);
		glVertex2f(TILE_SIZE / 3, -TILE_SIZE / 3);
		glEnd();
	}
		glPopMatrix();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
}

void Enemy::SetAtt(float firerate, int damage, int range, int health, float speed)
{
	SetHealth(health);
	SetDamage(damage);
	SetFireRate(firerate);
	SetRange(range);
	SetSpeed(speed);
}

void Enemy::Update(std::vector<Enemy *> eList, std::vector<Tower *> tList, float dt, int laneSwap)
{
	for (std::vector<Enemy *>::iterator it = eList.begin(); it != eList.end(); ++it)
	{
		Enemy *creep = *it;
		if (creep->GetActive() == true)
		{	
			//Update movement
			if (creep->GetFire() == false)
			{	
				int random = rand() % 2 + 1;

				creep->offsetX = creep->GetPos().x;
				creep->tilesTravelled = (creep->offsetX / TILE_SIZE);
				creep->offsetY = (creep->GetPos().y - (TILE_SIZE/2)) / TILE_SIZE; //current y tile

				if(creep->pattern > 1)
					creep->pattern = 0;

				//Enemy movement
				if(creep->state == ENEMY_STATE::ENEMY_ADVANCE)
				{
					creep->SetPos(Vector3(creep->GetPos().x + creep->GetVel().x*dt, creep->GetPos().y, 0));
				}

				//Enemy change lane
				if(creep->state == ENEMY_STATE::ENEMY_LANE)
				{
					switch(creep->type)
					{
					case Enemy::ENEMY_3:
						creep->offsetY2 = laneSwap;
						if(creep->offsetY2 < creep->offsetY)
							creep->dir = 0;
						else
							creep->dir = 1;
						break;

					case Enemy::ENEMY_4:
						if(!creep->called)
						{
							if(creep->offsetY > 1 && creep->offsetY < 5)
							{
								if(random == 1)
								{
									creep->offsetY2 = creep->offsetY - 1;
									creep->dir = 0;
								}
								else
								{
									creep->offsetY2 = creep->offsetY + 1;
									creep->dir = 1;
								}
							}
							else if(creep->offsetY == 5)
							{creep->offsetY2 = creep->offsetY - 1;
							creep->dir = 0;}
							else if(creep->offsetY == 1)
							{creep->offsetY2 = creep->offsetY + 1;
							creep-> dir = 1;}

							creep->called = true;
						}
						break;

					}

					if(creep->offsetY < creep->offsetY2 && creep->dir == 1)	//move down
					{
						creep->SetPos(Vector3(creep->GetPos().x, creep->GetPos().y - creep->GetVel().x*dt, 0));
					}

					else if(creep->offsetY >= creep->offsetY2 && creep->dir == 0)	//move up
					{
						creep->SetPos(Vector3(creep->GetPos().x, creep->GetPos().y + (creep->GetVel().x*dt), 0));
					}
					else
					{	
						if(creep->type == Enemy::ENEMY_3 || creep->type == Enemy::ENEMY_4)
						creep->SetPos(Vector3(creep->GetPos().x, ((creep->offsetY2)* TILE_SIZE + (TILE_SIZE/2)), 0));	//pop to middle of tile		1.set postion values to TILE_SIZE
						
						creep->currentTile -= 2;
						creep->called = false;
					}
				}
			}

			changeState(creep, laneSwap);
		}
	}
}

void Enemy::changeState(Enemy* creep, int laneswap)
{
	if(creep->GetFire())
	{
		if (creep->GetFireCounter() > 0)
			creep->state = Enemy::ENEMY_RELOADING;
		else
			creep->state = Enemy::ENEMY_ATTACK;
	}

	else if(!creep->GetFire()){
		//Change state
		switch (creep->type)
		{
		case Enemy::ENEMY_1:
			creep->state = ENEMY_STATE::ENEMY_ADVANCE;
			break;

		case Enemy::ENEMY_2:
			creep->state = ENEMY_STATE::ENEMY_ADVANCE;
			break;

		case Enemy::ENEMY_3:
			if(creep->offsetY != laneswap)
				creep->state = ENEMY_STATE::ENEMY_LANE;
			else 
				creep->state = ENEMY_STATE::ENEMY_ADVANCE;
			break;

		case Enemy::ENEMY_4:
			if(creep->currentTile == creep->tilesTravelled)
				creep->state = ENEMY_STATE::ENEMY_LANE;
			else
				creep->state = ENEMY_STATE::ENEMY_ADVANCE;
			break;

		case Enemy::ENEMY_5:
			creep->state = ENEMY_STATE::ENEMY_ADVANCE;
			break;

		}
	}
}

void Enemy::SetBuff(float buff)
{
	this->buff = buff;
}

void Enemy::SetSpeed(float speed)
{
	this->speed = speed;
}

float Enemy::GetBuff()
{
	return buff;
}

float Enemy::GetSpeed()
{
	return speed;
}

bool Enemy::collisionBox(int bulletX, int bulletY, int bossX, int bossY)
{
	if(bulletY <= bossY + TILE_SIZE && bulletY >= bossY - TILE_SIZE)
	{
		if(bulletX >= bossX)
			return true;
		else 
			return false;
	}
	else 
		return false;
}

bool Enemy::LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
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
	cout << "The file " << filename << " was opened" << endl;
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