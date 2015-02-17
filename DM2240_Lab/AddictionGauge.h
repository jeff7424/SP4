#pragma once

//#include "TextureImage.h"
#include "Vector3.h"
#include <GL\glut.h>
#include "PlayerInfo.h"


class CAddictionGauge
{
	private:
		Vector3 pos;
		int maxlength;
		bool active;
		int addiction;
		
	public:
		CAddictionGauge();
		~CAddictionGauge();
		//TextureImage AddictionTexture;
		PlayerInfo hello;

		void SetPosX(int x);
		void SetPosY(int y);
		void SetMaxLength(int l);
		void SetAddiction(int a);

		void Active(bool);
		int GetPosX();
		int GetPosY();
		int GetMaxLength();
		int GetAddiction();

		void initAddiction();

		bool Active();

		void DrawAddiction(int x, int y);
};

