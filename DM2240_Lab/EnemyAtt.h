#pragma once

#include "Units.h"
#include "EnemyProgress.h"'
#include "Map.h"
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class Enemy : public Units {
private:
	float buff;
	float speed;

public:

	int offsetX, offsetY; //current x tile, current y tile
	int offsetY2;//current y tile + 1
	int tilesTravelled, currentTile;
	bool called;

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
		ENEMY_ATTACK
	};

	ENEMY_STATE state;
	ENEMY_TYPE type;
	
	Enemy(ENEMY_TYPE type = ENEMY_1);
	~Enemy();

	void Update(float dt);
	void SetAtt(float firerate, int damage, int range, int health, float speed);
	void SetSpeed(float speed);

	void SetMovement (std::vector<Enemy *> eList, int TILE_SIZE, float dt, int laneSwap);
	void laneCheck (CMap * theMap);
	
	void SetBuff(float buff);
	float GetBuff();
	
	float GetSpeed();
};