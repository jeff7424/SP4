#pragma once

#include "Units.h"

class Enemy : public Units {
private:
	float buff;
	float speed;

public:
	enum ENEMY_TYPE
	{
		ENEMY_NONE,
		ENEMY_1,
		ENEMY_2,
		ENEMY_3,
		ENEMY_TOTAL,
	};

	enum ENEMY_YIELD
	{
		NME_Y1 = 25,
		NME_Y2 = 50,
		NME_Y3 = 100
	};

	ENEMY_TYPE type;
	
	Enemy(ENEMY_TYPE type = ENEMY_1);
	~Enemy();

	void Update(float dt);
	void SetAtt(float firerate, int damage, int range, int health, float speed);
	void SetSpeed(float speed);

	void SetBuff(float buff);
	float GetBuff();
	
	float GetSpeed();
};
