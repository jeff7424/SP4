#include "AddictionGauge.h"

CAddictionGauge::CAddictionGauge()
: pos(Vector3(0,0,0))
, maxlength(3)
, active(false)
, addiction(0)
{

}

CAddictionGauge::~CAddictionGauge()
{
}

void CAddictionGauge::SetPosX(int x)
{
	pos.x = x;
}

void CAddictionGauge::SetPosY(int y)
{
	pos.y = y;
}

void CAddictionGauge::SetMaxLength(int l)
{
	maxlength = l;
}

void CAddictionGauge::SetAddiction(int a)
{
	addiction = a;
}

void CAddictionGauge::Active(bool a)
{
	active = a;
}

int CAddictionGauge::GetPosX()
{
	return pos.x;
}

int CAddictionGauge::GetPosY()
{
	return pos.y;
}

int CAddictionGauge::GetMaxLength()
{
	return maxlength;
}

int CAddictionGauge::GetAddiction()
{
	return addiction;
}

bool CAddictionGauge::Active()
{
	return active;
}

void CAddictionGauge::initAddiction()
{
	maxlength = 3;
	addiction = 0;
}

void CAddictionGauge::DrawAddiction(int x, int y)
{
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);

	glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(x, y, 0);


		glBegin(GL_QUADS);
		glVertex2f(-1, -2);
		glVertex2f(-1, 50);
		glVertex2f(385, 50);
		glVertex2f(385, -2);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		/*glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, AddictionTexture.texID);*/
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(x, y, 0);
		

		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(0, 48);
			glVertex2f(addiction * 128, 48);
			glVertex2f(addiction * 128, 0);
		glEnd();
	glPopMatrix();
	
	

	//glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);
}



