#include "TowerAtt.h"
#include <GL/glut.h>
#include <fstream>
#include <string>

using namespace std;

Tower::Tower(TOWER_TYPE type)
: type(type)
, cost(0)
{
	 
}

Tower::~Tower()
{

}

void Tower::Init()
{

}

void Tower::SetAtt(float firerate, int cost, int damage, int range, int health)
{
	///*switch (type)
	//{
	//case Tower::TOWER_NORMAL:
		SetFireRate(firerate);
		SetCost(cost);
		SetDamage(damage);
		SetRange(range);
		SetHealth(health);
	/*	break;
	case Tower::TOWER_CANNON:
		SetFireRate(5);
		SetCost(200);
		SetDamage(8);
		SetRange(300);
		SetHealth(20);
		break;
	case Tower::TOWER_SLOW:
		SetFireRate(4);
		SetCost(150);
		SetDamage(2);
		SetRange(450);
		SetHealth(15);
		break;
	case Tower::TOWER_SHOCK:
		SetFireRate(0.5f);
		SetCost(150);
		SetDamage(3);
		SetRange(200);
		SetHealth(10);
		break;
	}*/
}

void Tower::SetCost(int cost)
{
	this->cost = cost;
}

int Tower::GetCost()
{
	return cost;
}

void Tower::DrawTowerLevel()
{
	for (int i = 0; i <= GetLevel(); i++)
	{
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 0.0f);
		glPopMatrix();
	}
}

void Tower::Update(float dt)
{

}

void Tower::Upgrade()
{
	if (GetLevel() < 3)
	{
		SetDamage(GetDamage() * 1.5f);
		SetHealth(GetHealth() * 1.5f);
		SetCost(GetCost() + 25);
		SetLevel(GetLevel() + 1);
	}
}

void Tower::DrawLevel()
{
	glPushMatrix();
	glTranslatef(GetPos().x - 40, GetPos().y - 50, GetPos().z);
	for (int lvl = 0; lvl < GetLevel(); lvl++)
	{
		glColor3f(1.0f, 1.0f, 0.0f);
		glTranslatef(15, 0, 0);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, 10);
		glVertex2f(10, 10);
		glVertex2f(10, 0);
		glEnd();
	}
	glPopMatrix();
}