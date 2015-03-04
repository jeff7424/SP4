#include "Units.h"
#include <GL\glut.h>

Units::Units()
: health(0.0f)
, maxhealth(0.0f)
, damage(0)
, level(1)
, range(0)
, fire(false)
, firerate(0)
, fireCounter(0)
{
	//for (int i = 0; i < MAX_PARTICLE; i++)
	//{
	//	float x = (((float)rand() / RAND_MAX) - 0.5f) / 30;
	//	float y = ((float)rand() / RAND_MAX) / 20;
	//	float z = ((float)rand() / RAND_MAX) / 3;
	//	ps[i].vel = Vector3(x, y, z); // Direction
	//	x = (((float)rand() / RAND_MAX) - 0.5f) / 15;
	//	y = (((float)rand() / RAND_MAX) - 0.5f) / 30;
	//	z = (((float)rand() / RAND_MAX) - 0.5f) / 15;
	//	ps[i].pos = Vector3(x, y, z); // Position
	//	x = ((float)rand() / RAND_MAX);
	//	y = ((float)rand() / RAND_MAX);
	//	z = ((float)rand() / RAND_MAX);
	//	float a = ((float)rand() / RAND_MAX);
	//	ps[i].r = x; ps[i].g = y; ps[i].b = z; //Color
	//	ps[i].life = a;
	//}
}

Units::~Units()
{

}

void Units::SetHealth(float health)
{
	this->health = health;
}

void Units::SetMaxHealth(float maxhealth)
{
	this->maxhealth = maxhealth;
}

void Units::SetDamage(int damage)
{
	this->damage = damage;
}

void Units::SetLevel(int level)
{
	this->level = level;
}

void Units::SetRange(int range)
{
	this->range = range;
}

void Units::SetFire(bool fire)
{
	this->fire = fire;
}

void Units::SetFireRate(float firerate)
{
	this->firerate = firerate;
}

void Units::SetFireCounter(float firecounter)
{
	this->fireCounter = firecounter;
}

float Units::GetHealth()
{
	return health;
}

float Units::GetMaxHealth()
{
	return maxhealth;
}

int Units::GetDamage()
{
	return damage;
}

int Units::GetLevel()
{
	return level;
}

int Units::GetRange()
{
	return range;
}

bool Units::GetFire()
{
	return fire;
}

float Units::GetFireRate()
{
	return firerate;
}

float Units::GetFireCounter()
{
	return fireCounter;
}

void Units::DrawHealthBar()
{
	glDisable(GL_BLEND);
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(GetPos().x - 48, GetPos().y - 48, 0);
	glScalef(2, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0 - 1, 0 - 1);
	glVertex2f(48 + 1, 0 - 1);
	glVertex2f(48 + 1, 5 + 1);
	glVertex2f(0 - 1, 5 + 1);
	glEnd();
	glColor3f(0.2f, 0.2f, 0.6f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(48, 0);
	glVertex2f(48, 5);
	glVertex2f(0, 5);
	glEnd();
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f((health / maxhealth) * 48, 0);
	glVertex2f((health / maxhealth) * 48, 5);
	glVertex2f(0, 5);
	glEnd();
	glPopMatrix();
	glPopMatrix();
	glEnable(GL_BLEND);
}

void Units::DrawDeath()
{
	/*glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	glEnable(GL_BLEND);
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (ps[i].pos.x < -100 || ps[i].pos.x > 100)
		{
			ps[i].pos.x = 0;
		}
		if (ps[i].pos.y > 100)
		{
			ps[i].pos.y = 0;
		}
		ps[i].pos = ps[i].pos + ps[i].vel;
		ps[i].life -= 0.01f;
		if (ps[i].life < 0.005f)
		{
			ps[i].life = rand() % 5;
			ps[i].pos.x = 0;
			ps[i].pos.y = 0;
		}
		if (ps[i].life > 0.0f)
		{
			glPushMatrix();
			glTranslatef(ps[i].pos.x, ps[i].pos.y - 20, 0.0f);
			glColor4f(0.3f, 0.3f, 0.3f, ps[i].life);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, -5.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0f, -5.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0f, 5.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0f, 5.0f, 0.0f);
			glEnd();
			glPopMatrix();
		}
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);

	glPopMatrix();*/
}