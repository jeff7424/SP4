#pragma once
#include <vector>
#include "Vector3.h"
#include "Bullet.h"
#include "TextureImage.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>


#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 672

class CMiniGame

{
private:

	std::vector<Bullet *> MGBList; // Vector list for bullets
	void spawnbullet(int x , int y);
	void RenderBackground();
	void RenderCharacter();
	Vector3 playerposition;
	float cooldown;
	//Scroll Mini Game
	float offsetY, offsetY2;
	int timer;
	int mggold;

public:

	Bullet* newBullet;
	CMiniGame();
	void update(float dt);
	void Render();
	void SetPos(Vector3);
	Vector3 GetPos();
	void SetTimer(int t);
	int GetTimer();
	void SetMGGold(int g);
	int GetMGGold();
	bool winminigame;
	bool loseminigame;

	//For Mini Game
	void RenderBullets();
	void RenderMiniGame();
	void RenderMGBackground();
	void RenderMGCharacter();
	void RenderMGBlood();
	void RenderMGGun();
	void RenderMGWinScreen();
	void RenderMGLoseScreen();
	bool collision(int bulletx, int bullety, int playerx, int playery);

	bool LoadTGA(TextureImage *texture, char *filename);
	TextureImage MGCharacterTexture;
	TextureImage MGBackgroundTexture;
	TextureImage MGBloodTexture;
	TextureImage MGGunTexture;
	TextureImage MGWinTexture;
	TextureImage MGLoseTexture;
	~CMiniGame();
};

