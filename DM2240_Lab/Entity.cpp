#include "Entity.h"

Entity::Entity()
: pos(0, 0, 0)
, vel(0, 0, 0)
, scale(0, 0, 0)
, active(false)
{

}

Entity::~Entity()
{

}

void Entity::SetPos(Vector3 pos)
{
	this->pos = pos;
}

void Entity::SetVel(Vector3 vel)
{
	this->vel = vel;
}

void Entity::SetScale(Vector3 scale)
{
	this->scale = scale;
}

void Entity::SetActive(bool active)
{
	this->active = active;
}

Vector3 Entity::GetPos()
{
	return pos;
}

Vector3 Entity::GetVel()
{
	return vel;
}

Vector3 Entity::GetScale()
{
	return scale;
}

bool Entity::GetActive()
{
	return active;
}

void Entity::Update()
{

}