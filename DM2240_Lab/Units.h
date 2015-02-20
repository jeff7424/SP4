#pragma once

#include "Entity.h"

//#define MAX_PARTICLE 100
//
//typedef struct
//{
//	float r, g, b;
//	float life;
//	Vector3 pos, vel;
//} Particle;

class Units : public Entity {
private:
	int health;
	int damage;
	int level;
	int range;
	bool fire;
	float firerate;
	float fireCounter;

public:
	Units();
	~Units();

	void SetHealth(int health);
	void SetDamage(int damage);
	void SetLevel(int level);
	void SetRange(int range);
	void SetFire(bool fire);
	void SetFireRate(float firerate);
	void SetFireCounter(float firecounter);

	int GetHealth();
	int GetDamage();
	int GetLevel();
	int GetRange();
	bool GetFire();
	float GetFireRate();
	float GetFireCounter();

	void DrawHealthBar();
	void DrawDeath();

	//Particle ps[MAX_PARTICLE];
};