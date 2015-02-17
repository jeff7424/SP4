#pragma once
#include "Entity.h"
class Powerup : public Entity
{
private:
	int damage;
	int cost;

public:
	enum POWER_TYPE
	{
		POWER_NONE,
		POWER_CLEAR,
		POWER_LANE,
		POWER_DOWN,
		POWER_TOTAL,
	};

	POWER_TYPE type;
	
	Powerup(POWER_TYPE type);
	~Powerup();

	void SetType(POWER_TYPE type);

	int GetDmg();

	int GetCost();
};

