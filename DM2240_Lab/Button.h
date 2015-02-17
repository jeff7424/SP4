#pragma once

#include "Vector3.h"
#include "TextureImage.h"

class Button {
private:
	bool isHover;
	bool isClick;

	Vector3 Position;
	Vector3 Size;

	TextureImage Image;
	TextureImage Image_Hover;

public:
	Button();
	Button(char *filename, char *filenamehover, const int posx, const int posy, const int sizex, const int sizey);
	~Button(); 

	void SetPosition(const int x, const int y);
	void SetSize(const int x, const int y);

	Vector3 GetPosition();
	Vector3 GetSize();

	void SetIsHover(int x, int y);
	void SetIsClick(bool isClick);

	bool GetIsClick();
	bool GetIsHover();

	bool LoadTGA(TextureImage *texture, char *filename);

	void Render();
};