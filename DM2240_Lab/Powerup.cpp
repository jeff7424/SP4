#include "Powerup.h"
#include <iostream>

using namespace std;

Powerup::Powerup(POWER_TYPE type)
:type(type)
, Cost(0)
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
		Duration = 0.1f;
		MaxDuration = Duration;
		Cooldown = 10.0f;
		break;
	case POWER_TYPE::POWER_SHIELD:
		Value = 50;
		Duration = 0.1f;
		MaxDuration = Duration;
		Cooldown = 10.0f;
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
		Cooldown = 60.0f;
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

bool Powerup::GetReady()
{
	return Ready;
}

float Powerup::GetValue()
{
	return Value;
}

float Powerup::GetDuration()
{
	return Duration;
}

void Powerup::SetCost(int Cost)
{
	this->Cost = Cost;
}

void Powerup::SetReady(bool Ready)
{
	this->Ready = Ready;
}

void Powerup::SetValue(float Value)
{
	this->Value = Value;
}

void Powerup::SetDuration(float Duration)
{
	this->Duration = Duration;
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
	glTranslatef(x - 22 / 2, y + 30, 0);
	glScalef(2, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0 - 1, 0 - 1);
	glVertex2f(20 + 1, 0 - 1);
	glVertex2f(20 + 1, 10 + 1);
	glVertex2f(0 - 1, 10 + 1);
	glEnd();
	glColor3f(0.2f, 0.2f, 0.6f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(20, 0);
	glVertex2f(20, 10);
	glVertex2f(0, 10);
	glEnd();
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f((Duration / MaxDuration) * 20, 0);
	glVertex2f((Duration / MaxDuration) * 20, 10);
	glVertex2f(0, 10);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glEnable(GL_BLEND);
}