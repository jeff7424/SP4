#pragma once

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <stdio.h>
#include <time.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include "MyMath.h"
#include "Mouse.h"
#include "GameState.h"				// Game State
#include "vector3.h"				// Vector3D
#include "Camera.h"					// Camera
#include "TextureImage.h"			// Textures
#include "PlayerInfo.h"				// Player
#include "Spawn.h"
#include "TowerAtt.h"
#include "EnemyAtt.h"				// Enemy
#include "Bullet.h"
#include "AudioPlayer.h"
#include "EnemyProgress.h"
#include "Powerup.h"
#include "Map.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 672
#define TILE_SIZE 96		
#define MAX_TILE 17

using namespace std;

class CPlayState : public CGameState
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
	void MouseMove (int x, int y);
	void MouseClick(int button, int state, int x, int y);
	
	static CPlayState* Instance() {
		return &thePlayState;
	}
	void drop();

protected:
	CPlayState(){}

private:
	static CPlayState thePlayState;

	theMouseInfo mouseInfo;

	int w; // Window Width
	int h; // Window Height
	int state;
	int WX;
	int WY;
	// Maps initialization
	int pos_x, pos_y;
	int mapOffset_x, mapOffset_y;
	int tileOffset_x, tileOffset_y;
	int mapFineOffset_x, mapFineOffset_y;
	int theNumOfTiles_Height;
	int theNumOfTiles_Width;
	int rearWallOffset_x, rearWallOffset_y;
	int rearWalltileOffset_x, rearWalltileOffset_y;
	int rearWallFineOffset_x, rearWallFineOffset_y;
	int selection; // tower selection
	int enemycounter; // numbers of enemies left
	// Inventory
	int ClearMapCounter;
	int ClearLaneCounter;
	int heroAnimationCounter; // animation counter
	int gamesave;
	int progress; // level progress
	int info; // mouse hover info
	int level;
	float spawntimer; // timer for enemy to spawn
	float ratio;
	float m_fps;
	float m_speed;
	float m_worldSizeX;
	float m_worldSizeY;
	bool playmusic;
	bool Tracklist;
	bool pause;
	bool soundon;
	bool startgame;
	bool upgrade;
	bool levelselect;
	bool next;
	bool back;
	bool levelloaded;
	bool saveandload;
	bool powerMap;
	bool powerLane;
	bool powerfired;
	bool myKeys[255];

	std::vector<Bullet *> bulletList; // Vector list for bullets
	std::vector<Enemy *> enemyList; // Vector list for creeps
	std::vector<Tower *> towerList; // Vector list for towers
	std::vector<Spawn *> spawnList; // Vector list for towers
	std::vector<Powerup *> powerList; // Vector list for power ups

	std::vector<Tower *> towerClone;
	std::vector<Enemy *> enemyClone;

	// FetchGO
	Bullet* FetchBullet();
	Enemy* FetchEnemy();
	Tower* FetchTower();
	Spawn* FetchSpawn();
	Powerup* FetchPower();
	Tower m_ghost; // ghost

	PlayerInfo *player; // Player profile
	CMap *theMap; // CSV Map
	Camera *Cam; // Camera
	CEnemyProgress *tEnemyProgress;

	// Texture mapping
	//GLuint texture[24];
	// Insert texture here																						   	
	TextureImage BackgroundTexture[1];
	TextureImage TileMapTexture[5];
	TextureImage Icon[4];
	TextureImage CreepTexture[5];
	TextureImage BulletTexture[5];
	TextureImage TowerTexture[5];
	TextureImage Music[1];
	TextureImage Upgrade[1];
	TextureImage Quit[3];
	TextureImage Heart[1];
	TextureImage Story[1];
	TextureImage Power[2];
	TextureImage PowerBoom[2];

	void RenderTileMap();
	void moverlevel1(int x, int y);
	void mclicklevel1(int x, int y);
	bool LoadTGA(TextureImage *texture, char *filename);
	void loadlevel();
	void clearmap();
	void savegame();

	void DrawBullet(Bullet *bullet);
	void DrawEnemy(Enemy *creep);
	void DrawTower(Tower *tower);
	void RenderStringOnScreen(float x, float y, const char* quote);
	// Sounds
	AudioPlayer sound;
	ISoundEngine *se;
	void playSound(void);
	void shooting(bool firing);

	void RenderBackground();
	void Update(float dt);	// Collision checking and tower firing check

	void Save();		// Save game
	void Load();		// Load game
	void LoadAtt();		// Load towers and enemies attributes from text
	void LoadSpawn();	// Load enemies spawn data from text file
	void UpdateSpawn(); // Spawn the enemies from data get from text file

	// Power up
	void PowerOn();

	// Icons
	void tower1(); // Normal tower
	void tower2(); // Cannon tower
	void tower3(); // Lightning tower
	void tower4(); // Slow tower

	void newload();

	// Cursors
	void RenderUpgrade(int x, int y); // Hover to tower to show upgrade icon
	void RenderInfo(int x, int y); // Hover to tower selection icon to show info

	// Menus
	void track();		// Tracklist

	void power1();
	void power2();

	void exitover();
	void powerTex(bool yay, int boo);
	void soundTypes(int type, bool death);

	void RenderHUD();
};