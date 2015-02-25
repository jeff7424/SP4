#pragma once

#include "TextureImage.h"

class PlayerInfo {
private:
	int health;
	int maxhealth;
	int shield;
	int maxshield;
	int gold;
	int bonus_currency;
	int QtyBaseHealth;
	int QtyShield;
	int QtyFireRate;
	int QtyDamage;

public:
	PlayerInfo();
	~PlayerInfo();

	void SetHealth(int health);
	void SetMaxHealth(int maxhealth);
	void SetShield(int shield);
	void SetMaxShield(int maxshield);
	void SetGold(int gold);
	void SetBonus(int bonus);
	void SetQtyBaseHealth(int QtyBaseHealth);
	void SetQtyShield(int QtyShield);
	void SetQtyFireRate(int QtyFireRate);
	void SetQtyDamage(int QtyDamage);

	int GetHealth();
	int GetMaxHealth();
	int GetShield();
	int GetMaxShield();
	int GetGold();
	int GetBonus();
	int GetQtyBaseHealth();
	int GetQtyShield();
	int GetQtyFireRate();
	int GetQtyDamage();

	void RenderHealthBar(const int x, const int y);
	void RenderShield(const int x, const int y);
};