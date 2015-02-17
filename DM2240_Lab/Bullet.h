#pragma once

#include "Entity.h"

class Bullet : public Entity {
private:
	int damage;
	int health;
	Vector3 radius;

public:

	enum BULLET_TYPE
	{
		GO_NONE,
		GO_NORMALBULLET,
		GO_CANNONBULLET,
		GO_SHOCKBULLET,
		GO_SLOWBULLET,
		GO_BOMBBULLET,
		GO_TOTAL, // must be last
	};
	BULLET_TYPE type;

	Bullet(BULLET_TYPE type = GO_NORMALBULLET);
	~Bullet();

	void SetDamage(int damage);
	void SetRadius(Vector3 radius);
	void SetHealth(int health);
	
	int GetHealth();
	Vector3 GetRadius();
	int GetDamage();

	void Update(float dt);
};