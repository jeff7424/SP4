#include "Bullet.h"

Bullet::Bullet(BULLET_TYPE type)
: type(type)
, health(1)
, radius(Vector3(50,5,0))
, damage(0)
{

}

Bullet::~Bullet()
{

}

void Bullet::Update(float dt)
{
	
}

void Bullet::SetDamage(int damage)
{
	this->damage = damage;
}

void Bullet::SetRadius(Vector3 radius)
{
	this->radius = radius;
}

int Bullet::GetDamage()
{
	return damage;
}

Vector3 Bullet::GetRadius()
{
	return radius;
}

void Bullet::SetHealth(int health)
{
	this->health = health;
}

int Bullet::GetHealth()
{
	return health;
}