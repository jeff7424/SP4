#include <math.h>
#include "freeglut.h"

#include <fstream>
using namespace std;

#include "loadTGA.h"



// only for uncompressed file
bool LoadTGA(TextureImage *texture, char *filename)				// load TGA file to memory
{
	GLubyte		TGAheader[ 12 ] = {0,0,2,0,0,0,0,0,0,0,0,0};	// uncompressed TGA header
	GLubyte		TGAcompare[ 12 ];								// for comparing TGA header
	GLubyte		header[ 6 ];									// first 6 useful header bytes
	GLuint		bytesPerPixel;								    // number of bytes per pixel in TGA gile
	GLuint		imageSize;									    // for setting memory
	GLuint		temp;										    // temporary variable
	GLuint		type=GL_RGBA;								    // default GL mode (32 bits per pixel)

	FILE *file = fopen( filename, "rb" );						// open TGA file
	if(	file == NULL ||										    // check file exist
		fread( TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare ) ||	// are there 12 bytes to read?
		memcmp( TGAheader,TGAcompare,sizeof(TGAheader) )!=0				  ||	// does header match what we want
		fread(header,1,sizeof(header),file)!=sizeof(header))				    // if so read next 6 header bytes
	{
		if (file == NULL)									// check file exist 
		{
			return false;
		}
		else
		{
			fclose(file);									// close file on failure
			return false;								
		}
	}
	texture->width  = header[1] * 256 + header[0];			// determine TGA width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];			// determine TGA height	(highbyte*256+lowbyte)
 	if(	texture->width	<=0	||								// is width <= 0
		texture->height	<=0	||								// is height <=0
		(header[4]!=24 && header[4]!=32))					// is TGA 24 or 32 Bit
	{
		fclose(file);										// close file on failure
		return false;										
	}

	texture->bpp	= header[4];							// grab TGA's bits per pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;						//divide by 8 to get bytes per pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// calculate memory required for TGA data
	texture->imageData=(GLubyte *)malloc(imageSize);		// reserve memory to hold TGA data
	if(	texture->imageData==NULL ||							// does storage memory exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	// does image size match memory reserved?
	{
		if(texture->imageData!=NULL)						// check image data loaded
			free(texture->imageData);						// if yes, release image data
		fclose(file);										// close file
		return false;									
	}
	for(int i=0; i<int(imageSize); i+=bytesPerPixel)		// loop through image data
	{														// swaps 1st and 3rd bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];							// temporarily store value at image data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// set 1st byte to value of 3rd byte
		texture->imageData[i + 2] = temp;					// set 3rd byte to value in 'temp' (1st byte value)
	}
	fclose(file);											// close file

	// Build A Texture From The Data
    	glGenTextures(1, &texture->id);					        // generate texture id
	glBindTexture(GL_TEXTURE_2D, texture->id);	

	int channels;
   	if (type==GL_RGB) channels = 3;
    	else channels = 4;

	if (texture[0].bpp==24)									// was TGA 24 bits
		type=GL_RGB;										// if yes set 'type' to GL_RGB

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);		
	// build Mipmaps for various distances 
	gluBuild2DMipmaps(GL_TEXTURE_2D, channels, texture->width,  texture->height, type, GL_UNSIGNED_BYTE, texture->imageData);
	
	return true;						
}