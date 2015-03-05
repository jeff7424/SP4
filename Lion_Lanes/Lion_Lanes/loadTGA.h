//Step 1: Split this file up into .h and .cpp, remember to include the appropriate headers

#pragma once

#include <fstream>
using namespace std;

typedef struct
{
	GLubyte	*imageData;			// image data
	GLuint	bpp;				// image color depth ( bits per pixel )
	GLuint	width;				// image width
	GLuint	height;				// image height
	GLuint	id;				// texture id
} TextureImage;
 
// only for uncompressed file
bool LoadTGA(TextureImage *texture, char *filename);			// load TGA file to memory

//TextureImage textures[9];

