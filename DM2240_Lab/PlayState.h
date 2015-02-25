#ifndef PLAYSTATE_H_
#define PLAYSTATE_H_

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
#include "Tank.h"
#include "Bullet.h"
#include "AudioPlayer.h"
#include "EnemyProgress.h"
#include "Powerup.h"
#include "Map.h"
#include "Button.h"
#include "BG.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 672
#define TILE_SIZE 96		
#define MAX_TILE 17

using namespace std;

class Tank;
class Tower;

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
	int laneCheck ();
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

	std::vector<Bullet*> &GetBulletList(void);

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
	int theNumOfTiles_Height;
	int theNumOfTiles_Width;
	int selection; // tower selection
	int enemycounter; // numbers of enemies left
	// Inventory
	int heroAnimationCounter; // animation counter
	int progress; // level progress
	int info; // mouse hover info
	int level;
	float spawntimer; // timer for enemy to spawn
	float ratio;
	float m_fps;
	float m_speed;
	bool playmusic;
	bool pause;
	bool exitmenu;
	bool soundon;
	bool startgame;
	bool upgrade;
	bool levelselect;
	bool levelloaded;
	bool saveandload;
	bool myKeys[255];

	//For Win and Lose Screens
	bool winscreen;
	bool losescreen;

	std::vector<Bullet *> bulletList; // Vector list for bullets
	std::vector<Enemy *> enemyList; // Vector list for creeps
	std::vector<Tower *> towerList; // Vector list for towers
	std::vector<Spawn *> spawnList; // Vector list for towers

	std::vector<Tower *> towerClone; // For tower stats loaded from txt
	std::vector<Enemy *> enemyClone; // For enemy stats loaded from txt

	Powerup *Shield;
	Powerup *BaseHealth;
	Powerup *Firerate;
	Powerup *Damage;
	Powerup *Backup_Tank;

	Button *Button_Pause;
	Button *PauseMenu_Resume;
	Button *PauseMenu_Restart;
	Button *PauseMenu_Exit;
	Button *ExitMenu_Yes;
	Button *ExitMenu_No;

	Button *Power_Shield;
	Button *Power_BaseHealth;
	Button *Power_Firerate;
	Button *Power_Damage;
	Button *Power_BackupTank;

	//For Win Lose Screen Buttons
	Button *WinLose_MainMenu;
	Button *WinLose_RestartLevel;
	Button *WinLose_NextLevel;
	Button *WinLose_Shop;
	
	//Mini Game
	Button *WinLose_MiniGame;

	Button *Unit_Infantry;
	Button *Unit_Tank;
	Button *Unit_Heavy;
	Button *Unit_Sniper;
	Button *Unit_Mine;
	Button *Unit_Barricade;

	Button *Bonus_Attack;
	Button *Bonus_Armour;
	Button *Bonus_Dollar;
	BG *Shop_BG;

	float Bonus_MultAttack;
	float Bonus_MultArmour;
	float Bonus_MultDollar;

	// FetchGO
	Bullet* FetchBullet(Tower *tower);
	Enemy* FetchEnemy();
	Tower* FetchTower();
	Spawn* FetchSpawn();
	//Tower m_ghost; // ghost

	PlayerInfo *player; // Player profile
	CMap *theMap; // CSV Map
	Camera *Cam; // Camera
	CEnemyProgress *tEnemyProgress;
	Enemy* theEnemy;
	Tank* backupTank; // Tank power up

	// Texture mapping
	//GLuint texture[24];
	// Insert texture here																						   	
	TextureImage BackgroundTexture[1];
	//TextureImage Icon[6];
	TextureImage CreepTexture[3];
	
	TextureImage Upgrade[1];
	TextureImage WinScreenTexture;
	TextureImage LoseScreenTexture;
	TextureImage PauseMenu;
	TextureImage ExitMenu;

	void RenderTileMap();
	void mclicklevel1(int x, int y);
	bool LoadTGA(TextureImage *texture, char *filename);
	void loadlevel();
	void clearmap();

	void DrawEnemy(Enemy *creep);
	void RenderStringOnScreen(float x, float y, const char* quote);
	// Sounds
	AudioPlayer sound;
	ISoundEngine *se;
	void playSound(int a);
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

	// Cursors
	void RenderUpgrade(int x, int y); // Hover to tower to show upgrade icon
	void RenderInfo(int x, int y); // Hover to tower selection icon to show info

	// Menus
	void exitover();
	void soundTypes(int type, bool death);

	void RenderHUD();
	void RenderPauseMenu();
	void RenderExitMenu();

	//For Win and Lose Screens
	void RenderWinScreen(void);
	void RenderLoseScreen(void);

};
#endif
