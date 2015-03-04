#ifndef ENEMYATT_H_
#define ENEMYATT_H_

//#include "PlayState.h"
#include "Units.h"
#include "TextureImage.h"
#include "Bullet.h"
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>
#include <time.h>       /* time */

class Tower;
class Units;

class Enemy : public Units {
private:
	float buff;
	float speed;
	//int heroAnimationCounter;

	

public:

	enum ENEMY_TYPE
	{
		ENEMY_NONE,
		ENEMY_1,	//infantry
		ENEMY_2,	//bike	 (moevment)
		ENEMY_3,	//sniper (ranged)
		ENEMY_4,	//heavy  (ranged)(movement)
		ENEMY_TOTAL,
	};

	enum ENEMY_STATE
	{
		ENEMY_LANE,
		ENEMY_ADVANCE,
		ENEMY_ATTACK,
		ENEMY_RELOADING
	};

	enum ENEMY_YIELD
	{
		NME_Y1 = 25,
		NME_Y2 = 50,
		NME_Y3 = 100,
		NME_Y4 = 125
	};

	ENEMY_TYPE type;
	ENEMY_STATE state;

	Enemy(ENEMY_TYPE type = ENEMY_1);
	~Enemy();

	TextureImage CreepTexture[2];

	int offsetX, offsetY; //current x tile, current y tile
	int offsetY2; //current y tile + 1
	int tilesTravelled, currentTile;
	int dir;
	bool called;

	void SetAtt(float firerate, int damage, int range, int health, float speed);
	void SetSpeed(float speed);

	void DrawEnemy(int heroAnimationCounter);
	void Update (std::vector<Enemy *> eList, std::vector<Tower *> tList, float dt, int laneSwap);
	void changeState(Enemy* creep, int laneswap);
	void SetBuff(float buff);
	float GetBuff();
	bool LoadTGA(TextureImage *texture, char *filename);

	float GetSpeed();
};

#endif
