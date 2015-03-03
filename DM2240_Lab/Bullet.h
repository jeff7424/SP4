#pragma once

#include "EnemyAtt.h"
#include "Entity.h"
#include "TextureImage.h"

class Bullet : public Entity {
private:
	int damage;
	int health;
	int speed;
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
		GO_ENEMYBULLET,
		GO_TOTAL, // must be last
	};
	BULLET_TYPE type;

	TextureImage texture;

	Bullet(BULLET_TYPE type = GO_NORMALBULLET);
	~Bullet();

	bool LoadTGA(TextureImage *texture, char *filename);
	void SetDamage(int damage);
	void SetHealth(int health);
	void SetSpeed(int speed);
	void SetRadius(Vector3 radius);

	int GetHealth();
	int GetDamage();
	int GetSpeed();
	Vector3 GetRadius();

	void Update(float dt);
	void Render();
	Vector3 GetTopLeft();
	Vector3 GetBottomRight();
};