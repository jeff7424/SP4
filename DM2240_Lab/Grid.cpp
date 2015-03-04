// Done by Heng Seng

#include "Grid.h"
#include "stdio.h"
#include <GL/glut.h>
#include <string>

CGrid::CGrid()
: r(0.0f)
, g(0.0f)
, b(0.0f)
, index_x(0)
, index_y(0)
, xSize(1)
, ySize(1)
, terrainType(0)
, Occupied(false)
, CursorHit(false)
{
}

CGrid::~CGrid()
{
}

void CGrid::Init(const int index_x, const int index_y, const int xSize, const int ySize, int terrainType)
{
	this->index_x = index_x;
	this->index_y = index_y;
	this->xSize = xSize;
	this->ySize = ySize;
	this->terrainType = terrainType;
	char *filename = "";
	switch (terrainType)
	{
	case 1:
		filename = "bin/textures/grid_tex1.tga";
		Occupied = false;
		break;
	case 2:
		filename = "bin/textures/grid_tex2.tga";
		Occupied = false;
		break;
	case 3:
		filename = "bin/textures/grid_tex3.tga";
		Occupied = true;
		break;
	}
	LoadTGA(&texture, filename);
}

void CGrid::RenderScene(void)
{
	SetColor();
	glPushMatrix();
	glTranslatef((float)index_x * xSize, (float)index_y * ySize, 0);
	//Draw the grid's surface
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(r, g, b, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture.texID);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0.0f, 0.0f);
	glTexCoord2f(0, 1); glVertex2f(xSize, 0.0f);
	glTexCoord2f(1, 1); glVertex2f(xSize, ySize);
	glTexCoord2f(1, 0); glVertex2f(0.0f, ySize);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();

	// Draw the lines of the grid
	glPushAttrib(GL_ENABLE_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(xSize, 0.0f);
	glVertex2f(xSize, 0.0f);
	glVertex2f(xSize, ySize);
	glVertex2f(xSize, ySize);
	glVertex2f(0.0f, ySize);
	glVertex2f(0.0f, ySize);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glLineWidth(1);
	glPopAttrib();
	glPopMatrix();
}

void CGrid::SetColor()
{
	if (!CursorHit)
	{
		this->r = 1.0f;
		this->g = 1.0f;
		this->b = 1.0f;
	}
	else if (CursorHit)
	{
		if (GetOccupied())
		{
			this->r = 1.0f;
			this->g = 0.0f;
			this->b = 0.0f;
		}
		else
		{
			this->r = 0.0f;
			this->g = 1.0f;
			this->b = 0.0f;
		}
	}
}

Vector3 CGrid::GetCenterPoint()
{
	return Vector3(index_x * xSize + (xSize / 2), index_y * ySize + (ySize / 2), 0);
}

Vector3 CGrid::GetTopLeft()
{
	return Vector3(index_x * xSize, index_y * ySize, 0);
}

Vector3 CGrid::GetBottomRight()
{
	return Vector3((index_x + 1) * xSize, (index_y + 1) * ySize, 0);
}

void CGrid::SetOccupied(bool Occupied)
{
	this->Occupied = Occupied;
}

bool CGrid::GetOccupied()
{
	return Occupied;
}

void CGrid::CursorOnGrid(const int x, const int y)
{
	if (x > GetTopLeft().x && x < GetBottomRight().x &&
		y > GetTopLeft().y && y < GetBottomRight().y)
	{
		CursorHit = true;
	}
	else
	{
		CursorHit = false;
	}
}

bool CGrid::LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
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