#include "Powerup.h"
#include <iostream>

using namespace std;

Powerup::Powerup(POWER_TYPE type)
:type(type)
, Cost(0)
//, BaseHealthIncrease(0)
//, Shield(50)
//, FireRateMultiply(1.5f)
//, DamageMultiply(1.5f)
, Ready(true)
, Value(0.0f)
, Duration(0.0f)
, MaxDuration(0.0f)
, Cooldown(0.0f)
{
	this->SetActive(false);
	switch (type)
	{
	case POWER_TYPE::POWER_INCREASEBASEHEALTH:
		Value = 50;
		break;
	case POWER_TYPE::POWER_SHIELD:
		Value = 50;
		break;
	case POWER_TYPE::POWER_FIRERATEMULTIPLY:
		Value = 1.5f;
		Duration = 10.0f;
		MaxDuration = Duration;
		Cooldown = 30.0f;
		break;
	case POWER_TYPE::POWER_DAMAGEMULTIPLY:
		Value = 1.5f;
		Duration = 10.0f;
		MaxDuration = Duration;
		Cooldown = 30.0f;
		break;
	case POWER_TYPE::POWER_TANKBACKUP:
		Duration = 10.0f;
		MaxDuration = Duration;
		Cooldown = 10.0f;
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

//int Powerup::GetBaseHealthIncrease()
//{
//	return BaseHealthIncrease;
//}
//
//int Powerup::GetShield()
//{
//	return Shield;
//}
//
//float Powerup::GetFireRateMultiply()
//{
//	return FireRateMultiply;
//}
//
//float Powerup::GetDamageMultiply()
//{
//	return DamageMultiply;
//}

float Powerup::GetValue()
{
	return Value;
}

bool Powerup::GetReady()
{
	return Ready;
}

void Powerup::SetCost(int Cost)
{
	this->Cost = Cost;
}

//void Powerup::SetBaseHealthIncrease(int BaseHealthIncrease)
//{
//	this->BaseHealthIncrease = BaseHealthIncrease;
//}
//
//void Powerup::SetShield(int Shield)
//{
//	this->Shield = Shield;
//}
//
//void Powerup::SetFireRateMultiply(float FireRateMultiply)
//{
//	this->FireRateMultiply = FireRateMultiply;
//}
//
//void Powerup::SetDamageMultiply(float DamageMultiply)
//{
//	this->DamageMultiply = DamageMultiply;
//}

void Powerup::SetValue(float Value)
{
	this->Value = Value;
}

float Powerup::GetDuration()
{
	return Duration;
}

void Powerup::Update(float dt)
{
	if (this->GetActive() && Ready == true)
	{
		Duration -= dt;
		if (Duration <= 0.0f)
		{
			Duration == 0.0f;
			this->SetActive(false);
			Ready = false;
		}
	}
	else
	{
		Duration += dt / (Cooldown / MaxDuration);
		if (Duration >= MaxDuration)
		{
			Duration = MaxDuration;
			Ready = true;
		}
	}
}

void Powerup::RenderDurationBar(int x, int y)
{
	glDisable(GL_BLEND);
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(x - MaxDuration / 2, y + 30, 0);
	glScalef(2, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0 - 1, 0 - 1);
	glVertex2f(MaxDuration + 1, 0 - 1);
	glVertex2f(MaxDuration + 1, 10 + 1);
	glVertex2f(0 - 1, 10 + 1);
	glEnd();
	glColor3f(0.2f, 0.2f, 0.6f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(MaxDuration, 0);
	glVertex2f(MaxDuration, 10);
	glVertex2f(0, 10);
	glEnd();
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(Duration, 0);
	glVertex2f(Duration, 10);
	glVertex2f(0, 10);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glEnable(GL_BLEND);
}