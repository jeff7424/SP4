#include "Entity.h"

Entity::Entity()
: pos(0, 0, 0)
, vel(0, 0, 0)
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

bool Entity::GetActive()
{
	return active;
}

void Entity::Update()
{

}