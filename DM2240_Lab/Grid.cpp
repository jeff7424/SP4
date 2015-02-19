#include "Grid.h"
#include "stdio.h"
#include <GL/glut.h>
#include <string>

CGrid::CGrid()
: r(0.0f)
, g(0.0f)
, b(0.0f)
, index_x(0)
, index_y(0)
, xSize(1)
, ySize(1)
, terrainType(0)
, ListOfObjects(NULL)
, CursorHit(false)
{
}


CGrid::~CGrid()
{
	DeleteObjects();
}

// Initialization
void CGrid::Init(const int index_x, const int index_y, const int xSize, const int ySize, int terrainType)
{
	this->index_x = index_x;
	this->index_y = index_y;
	this->xSize = xSize;
	this->ySize = ySize;
	this->terrainType = terrainType;
}

// Add a new object to this grid
void CGrid::AddObject(Tower *tower)
{
	// If grid has no object, then push into it
	if (ListOfObjects.size() < 1)
		ListOfObjects.push_back(tower);
}

void CGrid::RenderScene(void)
{
	int id = index_x * 7 + index_y;
	char tempid[4];
	itoa(id, tempid, 16);
	glPushMatrix();
	glTranslatef((float)index_x * xSize + 20, (float)index_y * ySize + 20, 0);
	RenderStringOnScreen(index_x, index_y, tempid);
	glPopMatrix();

	SetColor();
	glPushMatrix();
	glTranslatef((float)index_x * xSize, (float)index_y * ySize, 0);
	//Draw the grid's surface
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(r, g, b, 0.6f);
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(xSize, 0.0f, 0.0f);
	glVertex3f(xSize, ySize, 0.0f);
	glVertex3f(0.0f, ySize, 0.0f);
	glEnd();
	glDisable(GL_BLEND);
	glPopAttrib();

	// Draw the lines of the grid
	glPushAttrib(GL_ENABLE_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(10);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(xSize, 0.0f, 0.0f);
	glVertex3f(xSize, 0.0f, 0.0f);
	glVertex3f(xSize, ySize, 0.0f);
	glVertex3f(xSize, ySize, 0.0f);
	glVertex3f(0.0f, ySize, 0.0f);
	glVertex3f(0.0f, ySize, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(0.1);
	glPopAttrib();
	glPopMatrix();
}

// Render Objects
//void CGrid::RenderObjects()
//{
//	//glPushAttrib(GL_ENABLE_BIT);
//	//// Draw the grid and its list of objects
//	//for (int i = 0; i < (int)ListOfObjects.size(); i++)
//	//{
//	//	ListOfObjects[i]->Render();
//	//}
//	//glPopAttrib();
//}

// Delete object from this grid
void CGrid::DeleteObjects(void)
{
	for (int i = 0; i < (int)ListOfObjects.size(); i++)
	{
		delete ListOfObjects[i];
		ListOfObjects[i] = NULL;
	}
	ListOfObjects.clear();
}

void CGrid::SetColor()
{
	/*if (Quality == 0)
	{
	this->r = 0.0f;
	this->g = 1.0f;
	this->b = 0.0f;
	}
	else if (Quality == 1)
	{
	this->r = 1.0f;
	this->g = 1.0f;
	this->b = 0.0f;
	}
	else if (Quality == 2)
	{
	this->r = 1.0f;
	this->g = 0.0f;
	this->b = 0.0f;
	}*/
	/*if (ListOfObjects.size() > 0)
	{
	this->r = 1.0f;
	this->g = 0.0f;
	this->b = 0.0f;
	}
	else
	{
	this->r = 0.0f;
	this->g = 1.0f;
	this->b = 0.0f;
	}*/
	if (!CursorHit)
	{
		if (terrainType == 1)
		{
			this->r = 0.0f;
			this->g = 0.5f;
			this->b = 0.0f;
		}
		else if (terrainType == 2)
		{
			this->r = 0.0f;
			this->g = 0.0f;
			this->b = 0.0f;
		}
	}
	else if (CursorHit)
	{
		if (GetOccupied())
		{
			this->r = 1.0f;
			this->g = 0.0f;
			this->b = 0.0f;
		}
		else
		{
			this->r = 0.0f;
			this->g = 1.0f;
			this->b = 0.0f;
		}
	}

}

Vector3 CGrid::GetCenterPoint()
{
	return Vector3(index_x * xSize + (xSize / 2), index_y * ySize + (ySize / 2), 0);
}

Vector3 CGrid::GetTopLeft()
{
	return Vector3(index_x * xSize, index_y * ySize, 0);
}

Vector3 CGrid::GetBottomRight()
{
	return Vector3((index_x + 1) * xSize, (index_y + 1) * ySize, 0);
}

bool CGrid::GetOccupied()
{
	if (ListOfObjects.size() > 0)
		return true;
}

void CGrid::CursorOnGrid(const int x, const int y)
{
	if (x > GetTopLeft().x && x < GetBottomRight().x &&
		y > GetTopLeft().y && y < GetBottomRight().y)
	{
		CursorHit = true;
	}
	else
	{
		CursorHit = false;
	}
}

void CGrid::RenderStringOnScreen(float x, float y, const char* quote)
{
	int length = strlen(quote);
	glRasterPos2f(x, y);

	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, quote[i]);
	}
}