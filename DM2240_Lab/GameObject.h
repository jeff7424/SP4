#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_SINGLEBULLET,
		GO_STUNBULLET,
		GO_CANNONBULLET,
		GO_SHOCKBULLET,
		GO_TOTAL, // must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 color;
	bool active;
	float mass;
	int health;
	int damage;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_SINGLEBULLET);
	~GameObject();
};

#endif