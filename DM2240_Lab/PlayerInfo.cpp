#include "PlayerInfo.h"
#include <fstream>
#include <string>

using namespace std;

PlayerInfo::PlayerInfo()
: health(100)
, maxhealth(health)
, shield(0)
, maxshield(shield)
, gold(1000)
, bonus_currency(0)
, QtyBaseHealth(0)
, QtyShield(0)
, QtyFireRate(0)
, QtyDamage(0)
, QtyTank(0)
{

}

PlayerInfo::~PlayerInfo()
{

}

void PlayerInfo::SetHealth(int health)
{
	this->health = health;
}

void PlayerInfo::SetMaxHealth(int maxhealth)
{
	this->maxhealth = maxhealth;
}

void PlayerInfo::SetShield(int shield)
{
	this->shield = shield;
}

void PlayerInfo::SetMaxShield(int maxshield)
{
	this->maxshield = maxshield;
}

void PlayerInfo::SetGold(int gold)
{
	this->gold = gold;
}

void PlayerInfo::SetBonus(int bonus)
{
	this->bonus_currency = bonus;
}

void PlayerInfo::SetQtyBaseHealth(int QtyBaseHealth)
{
	this->QtyBaseHealth = QtyBaseHealth;
}

void PlayerInfo::SetQtyShield(int QtyShield)
{
	this->QtyShield = QtyShield;
}

void PlayerInfo::SetQtyFireRate(int QtyFireRate)
{
	this->QtyFireRate = QtyFireRate;
}

void PlayerInfo::SetQtyDamage(int QtyDamage)
{
	this->QtyDamage = QtyDamage;
}

int PlayerInfo::GetHealth()
{
	return health;
}

int PlayerInfo::GetMaxHealth()
{
	return maxhealth;
}

int PlayerInfo::GetShield()
{
	return shield;
}

int PlayerInfo::GetMaxShield()
{
	return maxshield;
}

int PlayerInfo::GetGold()
{
	return gold;
}

int PlayerInfo::GetBonus()
{
	return bonus_currency;
}

int PlayerInfo::GetQtyBaseHealth()
{
	return QtyBaseHealth;
}

int PlayerInfo::GetQtyShield()
{
	return QtyShield;
}

int PlayerInfo::GetQtyFireRate()
{
	return QtyFireRate;
}

int PlayerInfo::GetQtyDamage()
{
	return QtyDamage;
}

int PlayerInfo::GetQtyTank()
{
	return QtyTank;
}

void PlayerInfo::SetQtyTank(int QtyTank)
{
	this->QtyTank = QtyTank;
}

void PlayerInfo::RenderHealthBar(const int x, const int y)
{
	glDisable(GL_BLEND);
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(x, y, 0);
	glScalef(2, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0 - 1, 0 - 1);
	glVertex2f(maxhealth + 1, 0 - 1);
	glVertex2f(maxhealth + 1, 30 + 1);
	glVertex2f(0 - 1, 30 + 1);
	glEnd();
	glColor3f(0.2f, 0.2f, 0.6f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(maxhealth, 0);
	glVertex2f(maxhealth, 30);
	glVertex2f(0, 30);
	glEnd();
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(health, 0);
	glVertex2f(health, 30);
	glVertex2f(0, 30);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glEnable(GL_BLEND);
}

void PlayerInfo::RenderShield(const int x, const int y)
{
	glDisable(GL_BLEND);
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(x, y, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0 - 1, 0 - 1);
	glVertex2f(maxshield + 1, 0 - 1);
	glVertex2f(maxshield + 1, 30 + 1);
	glVertex2f(0 - 1, 30 + 1);
	glEnd();
	glColor3f(0.2f, 0.2f, 0.6f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(maxshield, 0);
	glVertex2f(maxshield, 30);
	glVertex2f(0, 30);
	glEnd();
	glPushMatrix();
	glColor3f(0.5f, 0.8f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(shield, 0);
	glVertex2f(shield, 30);
	glVertex2f(0, 30);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glEnable(GL_BLEND);
}