#include "TowerAtt.h"
#include <GL/glut.h>
#include <fstream>
#include <string>

using namespace std;

#define TILE_SIZE 96

Tower::Tower(TOWER_TYPE type)
: type(type)
, cost(0)
, Target(NULL)
, state(STATE_IDLE)
{
	char* filename = "";
	switch (type)
	{
	case TOWER_TYPE::TOWER_NORMAL:
		filename = "bin/tower/Heavy.tga";
		break;
	case TOWER_TYPE::TOWER_CANNON:
		filename = "bin/tower/Tank.tga";
		break;
	case TOWER_TYPE::TOWER_SLOW:
		filename = "bin/tower/Soldier.tga";
		break;
	case TOWER_TYPE::TOWER_SHOCK:
		filename = "bin/tower/Heavy.tga";
		break;
	case TOWER_TYPE::TOWER_MINE:
		filename = "bin/tower/mine.tga";
		break;
	case TOWER_TYPE::TOWER_BARRICADE:
		filename = "bin/tower/barricade.tga";
		break;
	}
	LoadTGA(&TowerTexture, filename);
}

Tower::~Tower()
{

}

bool Tower::LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
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

void Tower::SetAtt(float firerate, int cost, int damage, int range, int health)
{
	SetFire(false);
	SetFireRate(firerate);
	SetCost(cost);
	SetDamage(damage);
	SetRange(range);
	SetHealth(health);
}

void Tower::SetCost(int cost)
{
	this->cost = cost;
}

int Tower::GetCost()
{
	return cost;
}

void Tower::DrawTowerLevel()
{
	for (int i = 0; i <= GetLevel(); i++)
	{
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 0.0f);
		glPopMatrix();
	}
}

void Tower::ChangeState()
{
	if (state == STATE_IDLE)
	{
		if (Target != NULL && Target->GetActive() && Target->GetHealth() > 0)
		{
			float CurrentTarget = Target->GetPos().x - this->GetPos().x;
			if (this->GetRange() >= CurrentTarget && this->GetPos().y == Target->GetPos().y)
			{
				state = STATE_LOADING;
			}
		}
	}
	else if (state == STATE_LOADING)
	{
		if (Target != NULL && Target->GetActive() && Target->GetHealth() > 0)
		{
			float CurrentTarget = Target->GetPos().x - this->GetPos().x;
			if (this->GetRange() >= CurrentTarget && this->GetPos().y == Target->GetPos().y)
			{
				if (this->GetFireCounter() <= 0.0f)
				{
					state = STATE_ATTACK;
					this->SetFireCounter(this->GetFireRate());
					this->SetFire(true);
				}
			}
		}
		else if (Target == NULL || !Target->GetActive() || Target->GetHealth() <= 0)
		{
			state = STATE_IDLE;
			this->SetFire(false);
		}
	}
	else if (state == STATE_ATTACK)
	{
		if (Target != NULL && Target->GetActive() && Target->GetHealth() > 0)
		{
			float CurrentTarget = Target->GetPos().x - this->GetPos().x;
			state = STATE_LOADING;
			this->SetFire(false);
		}
		else if (Target == NULL || !Target->GetActive() || Target->GetHealth() <= 0)
		{
			state = STATE_IDLE;
			this->SetFire(false);
		}
	}
}

void Tower::Respond(float dt)
{
	switch (state)
	{
	case STATE_IDLE:
		break;
	case STATE_LOADING:
		{
			if (this->GetFireCounter() > 0.0f)
			{
				this->SetFireCounter(this->GetFireCounter() - dt);
			}
			else
			{
				this->SetFireCounter(0.0f);
			}
		}
		break;
	case STATE_ATTACK:
		{
			if (Target != NULL && Target->GetActive() && Target->GetHealth() > 0)
			{
				if (this->GetFire())
				{
					Bullet* bullet = new Bullet();
					bullet->SetActive(true);
					bullet->SetDamage(this->GetDamage());
					bullet->SetPos(this->GetPos());
					//bullet->SetVel(Target->GetPos() - this->GetPos());
					bullet->SetSpeed(400);
					CPlayState::Instance()->GetBulletList().push_back(bullet);
				}
			}
		}
		break;
	}
}

void Tower::Update(float dt)
{
	ChangeState();
	Respond(dt);
}

void Tower::Upgrade()
{
	if (GetLevel() < 3)
	{
		SetDamage(GetDamage() * 1.5f);
		SetHealth(GetHealth() * 1.5f);
		SetCost(GetCost() + 25);
		SetLevel(GetLevel() + 1);
	}
}

void Tower::DrawLevel()
{
	glPushMatrix();
	glTranslatef(GetPos().x - 40, GetPos().y - 50, GetPos().z);
	for (int lvl = 0; lvl < GetLevel(); lvl++)
	{
		glColor3f(1.0f, 1.0f, 0.0f);
		glTranslatef(15, 0, 0);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, 10);
		glVertex2f(10, 10);
		glVertex2f(10, 0);
		glEnd();
	}
	glPopMatrix();
}

void Tower::Render()
{
	DrawHealthBar();
	DrawLevel();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, TowerTexture.texID);
	glTranslatef(GetPos().x, GetPos().y, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(TILE_SIZE / 2, TILE_SIZE / 2);
	glTexCoord2f(0, 1); glVertex2f(TILE_SIZE / 2, -TILE_SIZE / 2);
	glTexCoord2f(1, 1); glVertex2f(-TILE_SIZE / 2, -TILE_SIZE / 2);
	glTexCoord2f(1, 0); glVertex2f(-TILE_SIZE / 2, TILE_SIZE / 2);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void Tower::GetTarget(std::vector<Enemy*> EnemyList)
{
	for (int i = 0; i < EnemyList.size(); ++i)
	{	
		if (EnemyList[i]->GetActive() && EnemyList[i]->GetHealth() > 0)
		{
			// if enemy in range
			if (this->GetPos().y == EnemyList[i]->GetPos().y && EnemyList[i]->GetPos().x - this->GetPos().x < this->GetRange())
			{
				// set target to this enemy
				Target = EnemyList[i];
				break;
			}
		}
		else if (!EnemyList[i]->GetActive() || EnemyList[i]->GetHealth() <= 0)
		{
			// else if enemy is dead, set target to null
			Target = NULL;
			break;
		}
	}
}