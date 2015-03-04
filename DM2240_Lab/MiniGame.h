#ifndef MINIGAME_H_
#define MINIGAME_H_

#pragma once

#include "GameState.h"
#include <stdio.h>
#include <mmsystem.h>
#include <vector>
#include "Vector3.h"
#include "Bullet.h"
#include "TextureImage.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "Camera.h"
#include "Mouse.h"
#include "AudioPlayer.h"
#include <iostream>
#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 672

class CMiniGame : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameStateManager* theGSM);
	void Update(CGameStateManager* theGSM);
	void Draw(CGameStateManager* theGSM);

	void changeSize(int w, int h);
	void inputKey(int key, int x, int y);
	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void MouseMove(int x, int y);
	void MouseClick(int button, int state, int x, int y);
	void SetInGame(bool ingame);

	void SetMGGold(int g);
	int GetMGGold();
	int mggold;
	static CMiniGame* Instance() {
		return &theMiniGame;
	}

protected:
	CMiniGame(){}

private:
	static CMiniGame theMiniGame;
	theMouseInfo mouseInfo;
	
	Vector3 playerposition;
	float cooldown;
	//Scroll Mini Game
	float offsetY, offsetY2;
	int timer;
	int currentscore;
	int w, h;
	bool winminigame;
	bool loseminigame;
	bool ingame;
	Camera* Cam;
	AudioPlayer* MiniGameMusic;
	AudioPlayer bgm;
	std::vector<Bullet *> MGBList; // Vector list for bullets
	
	void spawnbullet(int x, int y);
	void SetPos(Vector3);
	Vector3 GetPos();
	void SetTimer(int t);
	int GetTimer();
	void SetCurrentScore(int cs);
	int GetCurrentScore();

	//For Mini Game
	void RenderBullets();
	void RenderMiniGame();
	void RenderMGBackground();
	void RenderMGCharacter();
	void RenderMGWinScreen();
	void RenderMGLoseScreen();
	void RenderMGLoseScreen2();
	bool collision(int bulletx, int bullety, int playerx, int playery);
	void RenderStringOnScreen(float x, float y, const char* quote);
	bool LoadTGA(TextureImage *texture, char *filename);
	TextureImage MGCharacterTexture;
	TextureImage MGBackgroundTexture1;
	TextureImage MGBackgroundTexture2;
	TextureImage MGWinTexture;
	TextureImage MGLoseTexture;
	TextureImage MGLoseTexture2;
};
#endif
