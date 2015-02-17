#include "Powerup.h"
#include <iostream>

using namespace std;

Powerup::Powerup(POWER_TYPE type)
	:type(type)
	, damage(0)
	, cost(0)
{
}


Powerup::~Powerup()
{
}

void Powerup::SetType(POWER_TYPE type)
{
	switch (type)
	{
	case POWER_TYPE::POWER_CLEAR:
		this->damage = 80;
		this->cost = 500;
		break;
	case POWER_TYPE::POWER_LANE:
		this->damage = 80;
		this->cost = 150;
		break;
	case POWER_TYPE::POWER_DOWN:
		this->cost = 700;
		break;
	}
}

int Powerup::GetCost()
{
	return cost;
}


int Powerup::GetDmg()
{
	return damage;
}
