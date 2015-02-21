#pragma once

#include "Units.h"
#include "TextureImage.h"

class Tower : public Units {
private:
	int cost;

public:
	enum TOWER_TYPE
	{
		TOWER_NONE,
		TOWER_NORMAL,
		TOWER_CANNON,
		TOWER_SHOCK,
		TOWER_SLOW,
		TOWER_TOTAL,
	};

	TOWER_TYPE type;

	Tower(TOWER_TYPE type = TOWER_NORMAL);
	~Tower();

	TextureImage TowerTexture;
	bool LoadTGA(TextureImage *texture, char *filename);
	void Update(float dt);

	void SetAtt(float firerate, int cost, int damage, int range, int health);
	void SetCost(int cost);

	int GetCost();

	void DrawTowerLevel();

	void Upgrade();
	void DrawLevel();
	void Render();
};