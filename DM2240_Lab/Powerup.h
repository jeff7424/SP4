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
	bool GetReady();
	float GetValue();
	float GetDuration();

	void SetCost(int Cost);
	void SetReady(bool Ready);
	void SetValue(float Value);
	void SetDuration(float Duration);

	void Update(float dt);

	void RenderDurationBar(int x, int y);
};

