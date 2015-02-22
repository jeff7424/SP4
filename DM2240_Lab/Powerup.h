#pragma once
#include "Entity.h"
#include "TextureImage.h"

class Powerup : public Entity
{
private:
	int Cost;
	int BaseHealthIncrease;
	int Shield;
	float FireRateMultiply;
	float DamageMultiply;
	float Duration;

public:
	enum POWER_TYPE
	{
		POWER_NONE,
		POWER_INCREASEBASEHEALTH,
		POWER_SHIELD,
		POWER_FIRERATEMULTIPLY,
		POWER_DAMAGEMULTIPLY,
		POWER_TANKBACKUP,
		POWER_TOTAL,
	};

	POWER_TYPE type;
	
	Powerup(POWER_TYPE type);
	~Powerup();

	int GetCost();
	int GetBaseHealthIncrease();
	int GetShield();
	float GetFireRateMultiply();
	float GetDamageMultiply();

	void SetCost(int Cost);
	void SetBaseHealthIncrease(int BaseHealthIncrease);
	void SetShield(int Shield);
	void SetFireRateMultiply(float FireRateMultiply);
	void SetDamageMultiply(float DamageMultiply);

	void Update(float dt);
	float GetDuration();
};

