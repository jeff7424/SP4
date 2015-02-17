#include "Spawn.h"

Spawn::Spawn()
: second(0)
, type(0)
, active(false)
{

}

Spawn::~Spawn()
{

}

void Spawn::SetType(int type)
{
	this->type = type;
}

void Spawn::SetTime(float time)
{
	this->second = time;
}

void Spawn::SetActive(bool active)
{
	this->active = active;
}

float Spawn::GetTime()
{
	return second;
}

int Spawn::GetType()
{
	return type;
}

bool Spawn::GetActive()
{
	return active;
}