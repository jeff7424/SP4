#pragma once
#include "Entity.h"
#include "TextureImage.h"

class Powerup : public Entity
{
private:
	int Cost;
	bool Ready;
	float Value;
	float MaxDuration;
	float Duration;
	float Cooldown;

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
	float GetValue();
	bool GetReady();

	void SetCost(int Cost);
	void SetValue(float Value);

	void Update(float dt);
	float GetDuration();

	void RenderDurationBar(int x, int y);
};

