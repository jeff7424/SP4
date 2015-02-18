#include "Powerup.h"
#include <iostream>

using namespace std;

Powerup::Powerup(POWER_TYPE type)
:type(type)
, Cost(0)
, BaseHealthIncrease(0)
, Shield(50)
, FireRateMultiply(1.5f)
, DamageMultiply(1.5f)
{
}

Powerup::~Powerup()
{
}

void Powerup::SetType(POWER_TYPE type)
{
	switch (type)
	{
	case POWER_TYPE::POWER_INCREASEBASEHEALTH:

		break;
	case POWER_TYPE::POWER_SHIELD:

		break;
	case POWER_TYPE::POWER_FIRERATEMULTIPLY:
		
		break;
	case POWER_TYPE::POWER_DAMAGEMULTIPLY:
		
		break;
	
	}
}

int Powerup::GetCost()
{
	return Cost;
}

int Powerup::GetBaseHealthIncrease()
{
	return BaseHealthIncrease;
}

int Powerup::GetShield()
{
	return Shield;
}

float Powerup::GetFireRateMultiply()
{
	return FireRateMultiply;
}

float Powerup::GetDamageMultiply()
{
	return DamageMultiply;
}

void Powerup::SetCost(int Cost)
{
	this->Cost = Cost;
}

void Powerup::SetBaseHealthIncrease(int BaseHealthIncrease)
{
	this->BaseHealthIncrease = BaseHealthIncrease;
}

void Powerup::SetShield(int Shield)
{
	this->Shield = Shield;
}

void Powerup::SetFireRateMultiply(float FireRateMultiply)
{
	this->FireRateMultiply = FireRateMultiply;
}

void Powerup::SetDamageMultiply(float DamageMultiply)
{
	this->DamageMultiply = DamageMultiply;
}

//void Powerup::RenderIcon(const int x, const int y)
//{
//
//}