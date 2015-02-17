#include "Grid.h"
#include "stdio.h"
#include <GL/glut.h>

CGrid::CGrid()
: r(0.0f)
, g(0.0f)
, b(0.0f)
, index_x(0)
, index_y(0)
, xSize(1)
, ySize(1)
, ListOfObjects(NULL)
, Quality(0)
{
}


CGrid::~CGrid()
{
	DeleteObjects();
}

// Initialization
void CGrid::Init(const int index_x, const int index_y, const int xSize, const int ySize)
{
	this->index_x = index_x;
	this->index_y = index_y;
	this->xSize = xSize;
	this->ySize = ySize;
}

// Add a new object to this grid
void CGrid::AddObject(CObjectModel *theObject)
{
	ListOfObjects.push_back(theObject);
}

void CGrid::RenderScene(void)
{
	SetColor();
	glPushMatrix();
	glTranslatef((float)index_x * xSize, 0, (float)index_y * ySize);
	//Draw the grid's surface
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(r, g, b, 0.6f);
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(xSize, 0.0f, 0.0f);
	glVertex3f(xSize, 0.0f, ySize);
	glVertex3f(0.0f, 0.0f, ySize);
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
	glVertex3f(xSize, 0.0f, ySize);
	glVertex3f(xSize, 0.0f, ySize);
	glVertex3f(0.0f, 0.0f, ySize);
	glVertex3f(0.0f, 0.0f, ySize);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1);
	glPopAttrib();
	glPopMatrix();
}

// Render Objects
void CGrid::RenderObjects()
{
	glPushAttrib(GL_ENABLE_BIT);
	// Draw the grid and its list of objects
	for (int i = 0; i < (int)ListOfObjects.size(); i++)
	{
		ListOfObjects[i]->Render(Quality);
	}
	glPopAttrib();
}

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
	if (Quality == 0)
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
	}
}

Vector3D CGrid::GetCenterPoint()
{
	return Vector3D(index_x * xSize + (xSize / 2), 0, index_y * ySize + (ySize / 2));
}

Vector3D CGrid::GetBottomRight()
{
	return Vector3D(index_x * xSize, 0, index_y * ySize);
}

Vector3D CGrid::GetTopLeft()
{
	return Vector3D((index_x + 1) * xSize, 0, (index_y + 1) * ySize);
}