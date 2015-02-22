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
, Duration(0.0f)
{
	switch (type)
	{
	case POWER_TYPE::POWER_INCREASEBASEHEALTH:
		BaseHealthIncrease = 50;
		break;
	case POWER_TYPE::POWER_SHIELD:
		Shield = 50;
		break;
	case POWER_TYPE::POWER_FIRERATEMULTIPLY:
		FireRateMultiply = 1.5f;
		Duration = 10.0f;
		break;
	case POWER_TYPE::POWER_DAMAGEMULTIPLY:
		DamageMultiply = 1.5f;
		Duration = 10.0f;
		break;
	case POWER_TYPE::POWER_TANKBACKUP:
		Duration = 10.0f;
		break;
	}
}

Powerup::~Powerup()
{
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

float Powerup::GetDuration()
{
	return Duration;
}

void Powerup::Update(float dt)
{
	Duration -= dt;

	if (Duration <= 0.0f)
		Duration == 0.0f;
}