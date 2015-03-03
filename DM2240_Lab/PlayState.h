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
#include "MyMath.h"					// Math library
#include "Mouse.h"					// Mouse info
#include "GameState.h"				// Game State
#include "vector3.h"				// Vector3
#include "Camera.h"					// Camera
#include "TextureImage.h"			// Textures
#include "PlayerInfo.h"				// Player informations
#include "Spawn.h"					// Class to store when to spawn the enemy
#include "TowerAtt.h"				// Unit class
#include "EnemyAtt.h"				// Enemy class
#include "Tank.h"					// Tank power up class (FSM)
#include "Bullet.h"					// Projectile
#include "AudioPlayer.h"			// Class to manage audio (irrKlang)
#include "EnemyProgress.h"			// Enemy progression bar
#include "Powerup.h"				// Power class
#include "Map.h"					// Map class
#include "BG.h"						// Background
#include "Button.h"					// Buttons
#include "MiniGame.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 672
#define TILE_SIZE 96

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
	void Deathsounds();
	static CPlayState* Instance() {
		return &thePlayState;
	}
	void drop();

	std::vector<Bullet*> &GetBulletList(void);
	void SetLevel(int level);
	void LoadFromFile(bool load);

protected:
	CPlayState(){}

private:
	static CPlayState thePlayState;

	theMouseInfo mouseInfo;

	int w;						// Window Width
	int h;						// Window Height
	int state;					// Mouse click state
	int theNumOfTiles_Height;	// Number of tiles on y axis
	int theNumOfTiles_Width;	// Number of tiles on x axis
	int selection;				// Unit selection
	int heroAnimationCounter;	// animation counter
	static int progress;		// level progress
	int info;					// Mouse hover info
	float spawntimer;			// Timer for enemy to spawn
	float ratio;				// Aspect ratio
	float m_fps;				// FPS
	float m_speed;				
	bool playmusic;	
	bool pause;					// Trigger for pause menu
	bool exitmenu;				// Trigger for exit menu
	bool soundon;
	bool load;					// Trigger to load from txt file (save file)
	bool myKeys[255];			// Trigger for keyboard keys
	bool winscreen;				// Trigger win screen
	bool losescreen;			// Trigger lose screen
	bool audioplay;				// Audio option that reads from text file whether to play audio or not
	bool minigame;				// Trigger Mini Game
	std::vector<Bullet *> bulletList;	// Vector list for bullets
	std::vector<Enemy *> enemyList;		// Vector list for creeps
	std::vector<Tower *> towerList;		// Vector list for unit
	std::vector<Spawn *> spawnList;		// Vector list for spawning

	std::vector<Tower *> towerClone;	// For unit stats loaded from txt
	std::vector<Enemy *> enemyClone;	// For enemy stats loaded from txt

	// Power ups
	Powerup *Shield;
	Powerup *BaseHealth;
	Powerup *Firerate;
	Powerup *Damage;
	Powerup *Backup_Tank;

	// Buttons
	//For Win Lose Screen Buttons
	Button *WinLose_MainMenu;
	Button *WinLose_RestartLevel;
	Button *WinLose_NextLevel;
	Button *WinLose_Shop;
	Button *WinLose_MiniGame;

	// For in game buttons
	Button *Unit_Infantry;
	Button *Unit_Tank;
	Button *Unit_Heavy;
	Button *Unit_Sniper;
	Button *Unit_Mine;
	Button *Unit_Barricade;

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

	Button *Bonus_Attack;
	Button *Bonus_Armour;
	Button *Bonus_Dollar;

	Button *Bonus_PShd;
	Button *Bonus_PHealth;
	Button *Bonus_PFirerate;
	Button *Bonus_PDamage;
	Button *Bonus_PTank;

	BG *Shop_BG;

	float Bonus_MultAttack;
	float Bonus_MultArmour;
	float Bonus_MultDollar;

	// FetchGO
	Enemy* FetchEnemy();
	Tower* FetchTower();
	Spawn* FetchSpawn();

	PlayerInfo *player;					// Player profile
	CMap *theMap;						// CSV Map
	Camera *Cam;						// Camera
	CEnemyProgress *tEnemyProgress;		// Enemy progression bar
	Enemy* theEnemy;					
	Tank* backupTank;					// Tank power up
	CMiniGame *theMiniGame;				// Mini Game
	// Sounds
	AudioPlayer sound;
	ISoundEngine *se;
	ISound* object;

	// Texture mapping
	//GLuint texture[24];
	// Insert texture here																						   	
	TextureImage BackgroundTexture[1];
	TextureImage CreepTexture[3];
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
	void playSound(int a);
	void shooting(bool firing);

	void RenderBackground();
	void Update(float dt);	// Collision checking and tower firing check

	void Save();		// Save game
	void Load();		// Load game
	void LoadAtt();		// Load towers and enemies attributes from text
	void LoadSpawn();	// Load enemies spawn data from text file
	void UpdateSpawn(); // Spawn the enemies from data get from text file

	// Cursors
	void RenderInfo(int x, int y); // Hover to tower selection icon to show info

	// Menus
	void exitover();
	void soundTypes(int type);

	// Renderings
	void RenderHUD();
	void RenderPauseMenu();
	void RenderExitMenu();

	//For Win and Lose Screens
	void RenderWinScreen(void);
	void RenderLoseScreen(void);
};
#endif
