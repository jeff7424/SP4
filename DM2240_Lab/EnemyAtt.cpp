#include "EnemyAtt.h"
#include "TextureImage.h"

Enemy::Enemy(ENEMY_TYPE type)
: type(type)
, buff(0)
, speed(0)
{

}

Enemy::~Enemy()
{

}

void Enemy::SetAtt(float firerate, int damage, int range, int health, float speed)
{
	SetHealth(health);
	SetDamage(damage);
	SetFireRate(firerate);
	SetRange(range);
	SetSpeed(speed);
}

void Enemy::SetBuff(float buff)
{
	this->buff = buff;
}

void Enemy::SetSpeed(float speed)
{
	this->speed = speed;
}

float Enemy::GetBuff()
{
	return buff;
}

float Enemy::GetSpeed()
{
	return speed;
}

void Enemy::Update(float dt)
{

}