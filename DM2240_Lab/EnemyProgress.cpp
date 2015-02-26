#include "EnemyProgress.h"

CEnemyProgress::CEnemyProgress()
: pos(Vector3(0, 0, 0))
, minlength(0)
, active(false)
, enemycounter(0)
, maxenemycounter(0)
{

}

CEnemyProgress::~CEnemyProgress()
{
}

void CEnemyProgress::SetPosX(int x)
{
	pos.x = x;
}

void CEnemyProgress::SetPosY(int y)
{
	pos.y = y;
}

void CEnemyProgress::SetMinLength(int l)
{
	minlength = l;
}

void CEnemyProgress::SetEnemyCounter(int e)
{
	enemycounter = e;
}

void CEnemyProgress::SetMaxEnemyCounter(int e)
{
	maxenemycounter = e;
}

void CEnemyProgress::Active(bool a)
{
	active = a;
}

int CEnemyProgress::GetPosX()
{
	return pos.x;
}

int CEnemyProgress::GetPosY()
{
	return pos.y;
}

int CEnemyProgress::GetMinLength()
{
	return minlength;
}

int CEnemyProgress::GetEnemyCounter()
{
	return enemycounter;
}

int CEnemyProgress::GetMaxEnemyCounter()
{
	return maxenemycounter;
}

bool CEnemyProgress::Active()
{
	return active;
}

void CEnemyProgress::initEnemyCounter(int counter)
{
	minlength = 0;
	enemycounter = counter;
	maxenemycounter = enemycounter;
}

void CEnemyProgress::DrawEnemyCounter(int x, int y)
{
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
	glVertex2f(-1, -6);
	glVertex2f(-1, 20);
	glVertex2f(263, 20);
	glVertex2f(263, -6);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
	glVertex2f(0, -4);
	glVertex2f(0, 18);
	glVertex2f(enemycounter * 5, 18);
	glVertex2f(enemycounter * 5, -4);
	glEnd();
	glPopMatrix();
}



