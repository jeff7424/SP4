#pragma once

#include "Vector3.h"
#include "TextureImage.h"
#include "AudioPlayer.h"
class BG {
private:
	Vector3 Position;
	Vector3 Size;

	TextureImage Image;
	ISoundEngine *se;
public:
	BG();
	BG(char *filename, const int posx, const int posy, const int sizex, const int sizey);
	~BG(); 

	void SetPosition(const int x, const int y);
	void SetSize(const int x, const int y);

	Vector3 GetPosition();
	Vector3 GetSize();

	bool LoadTGA(TextureImage *texture, char *filename);

	void Render();
};