#include "Bullet.h"

Bullet::Bullet(BULLET_TYPE type)
: type(type)
, health(1)
, speed(200)
, radius(Vector3(50,5,0))
, damage(0)
{
	char* filename = "";
	switch (type)
	{
	case GO_NORMALBULLET:
		filename = "bin/textures/Soldier_shell.tga";
		break;
	case GO_CANNONBULLET:
		filename = "bin/textures/Tank_shell.tga";
		break;
	case GO_SHOCKBULLET:
		health = 3;
		filename = "bin/textures/Missile_shot.tga";
		break;
	case GO_SLOWBULLET:
		filename = "bin/textures/Sniper_shell.tga";
		break;
	case GO_MINIGAMEBULLET:
		filename = "bin/textures/mgbullet.tga";
		break;
	case GO_ENEMYBULLET:
		filename = "bin/textures/enemy_shell.tga";
		break;
	}
	LoadTGA(&texture, filename);
	this->SetVel(Vector3(1, 0, 0));
	this->SetScale(Vector3(24, 24, 1));
}

Bullet::~Bullet()
{
}

bool Bullet::LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
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

void Bullet::Update(float dt)
{
	if (this->GetActive())
	{
		this->SetPos(this->GetPos() + this->GetVel().Normalized() * (this->GetSpeed() * dt));
	}
}

void Bullet::SetHealth(int health)
{
	this->health = health;
}

void Bullet::SetDamage(int damage)
{
	this->damage = damage;
}

void Bullet::SetSpeed(int speed)
{
	this->speed = speed;
}

void Bullet::SetRadius(Vector3 radius)
{
	this->radius = radius;
}

int Bullet::GetDamage()
{
	return damage;
}

Vector3 Bullet::GetRadius()
{
	return radius;
}

int Bullet::GetHealth()
{
	return health;
}

int Bullet::GetSpeed()
{
	return speed;
}

void Bullet::Render()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texture.texID);
	glTranslatef(this->GetPos().x, this->GetPos().y, this->GetPos().z);
	glScalef(GetScale().x, GetScale().y, GetScale().z);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(0, 1); glVertex2f(-1.0f, 1.0f);
	glTexCoord2f(1, 1); glVertex2f(1.0f, 1.0f);
	glTexCoord2f(1, 0); glVertex2f(1.0f, -1.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

Vector3 Bullet::GetTopLeft()
{
	return Vector3(GetPos().x - GetScale().x, GetPos().y - GetScale().y, 0);
}

Vector3 Bullet::GetBottomRight()
{
	return Vector3(GetPos().x + GetScale().x, GetPos().y + GetScale().y, 0);
}