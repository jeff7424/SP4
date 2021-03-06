#pragma once

#include "PlayState.h"
#include "Units.h"
//#include "EnemyAtt.h"
#include "TextureImage.h"
#include "Bullet.h"
#include "Powerup.h"

class Tower : public Units {
private:
	int cost;
	Enemy *Target;
	
public:
	enum TOWER_TYPE
	{
		TOWER_NONE,
		TOWER_NORMAL,
		TOWER_CANNON,
		TOWER_SHOCK,
		TOWER_SLOW,
		TOWER_MINE,
		TOWER_BARRICADE,
		TOWER_TOTAL,
	};

	enum State {
		STATE_IDLE,
		STATE_LOADING,
		STATE_ATTACK
	};

	TOWER_TYPE type;
	State state;

	Tower(TOWER_TYPE type = TOWER_NORMAL);
	~Tower();

	TextureImage TowerTexture;
	bool LoadTGA(TextureImage *texture, char *filename);

	void ChangeState();
	void Respond(float dt, Powerup *firerate, Powerup *damage);
	void Update(float dt, Powerup *firerate, Powerup *damage);

	void SetAtt(float firerate, int cost, int damage, int range, int health);
	void SetCost(int cost);

	int GetCost();

	void Upgrade();
	void Render();
	void GetTarget(std::vector<Enemy*>);
	Vector3 GetTopLeft();
	Vector3 GetBottomRight();
	Enemy* ReturnTarget();
};