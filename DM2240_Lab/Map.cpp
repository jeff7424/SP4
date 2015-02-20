#include "Map.h"
#include "stdio.h"
#include <GL/glut.h>

CMap::CMap()
: theGrid(NULL)
, xPos(0)
, yPos(0)
, xSize(0)
, ySize(0)
, xNumOfGrid(0)
, yNumOfGrid(0)
{
	for (int i = 0; i < 70; i++)
	{
		Terrains[i] = 0;
	}
}

CMap::~CMap()
{
	/*for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < yNumOfGrid; j++)
		{
			theGrid[i * yNumOfGrid + j].DeleteObjects();
		}
	}*/
	delete[] theGrid;
}

bool CMap::LoadMap(const string mapName, const int xPos, const int yPos, const int xSize, const int ySize)
{
	if (LoadFile(mapName, xPos, yPos, xSize, ySize) == true)
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}

	return false;
}

bool CMap::LoadFile(const string mapName, const int xPos, const int yPos, const int xSize, const int ySize)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;
	int theColumnCounter = 0;
	int TerrainTypeCounter = 0;

	ifstream file(mapName.c_str());
	if (file.is_open())
	{
		int i = 0, k = 0;
		while (file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			// If this line is not a comment line, then process it
			if (!(aLineOfText.find("//") == NULL) && aLineOfText != "")
			{
				//if (theLineCounter == 0)
				//{
				//	// This is the first line of the map data file
				//	string token;
				//	istringstream iss(aLineOfText);
				//	while (getline(iss, token, ','))
				//	{
				//		// Count the number of columns
				//		theMaxNumOfColumns = atoi(token.c_str());
				//	}
				//}
				//else
				//{
				string token;
				istringstream iss(aLineOfText);
				while (getline(iss, token, ','))
				{
					Terrains[TerrainTypeCounter] = atoi(token.c_str());
					TerrainTypeCounter++;
					theColumnCounter++;
				}
				//}
				theLineCounter++;
			}
		}
	}

	TerrainTypeCounter = 0;
	// Initialization of the grids
	this->xPos = 0;
	this->yPos = 0;
	this->xNumOfGrid = theColumnCounter / theLineCounter;
	this->yNumOfGrid = theLineCounter;
	this->xSize = xSize;
	this->ySize = ySize;
	theGrid = new CGrid[xNumOfGrid * yNumOfGrid];

	for (int j = 0; j < yNumOfGrid; j++)
	{
		for (int i = 0; i < xNumOfGrid; i++)
		{
			theGrid[i * yNumOfGrid + j].Init(i, j, xSize, ySize, Terrains[TerrainTypeCounter]);
			TerrainTypeCounter++;
		}
	}

	return true;
}

void CMap::Init(const int xPos, const int yPos, const int xSize, const int ySize, const int xNumOfGrid, const int yNumOfGrid, int terrainType)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->xSize = xSize;
	this->ySize = ySize;
	this->xNumOfGrid = xNumOfGrid;
	this->yNumOfGrid = yNumOfGrid;
	theGrid = new CGrid[xNumOfGrid * yNumOfGrid];

	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < yNumOfGrid; j++)
		{
			theGrid[i * yNumOfGrid + j].Init(i, j, xSize, ySize, terrainType);
		}
	}

}

CGrid *CMap::GetGrid(const int xIndex, const int yIndex)
{
	return &theGrid[xIndex * yNumOfGrid + yIndex];
}

//void CSpatialPartition::RenderScene(Vector3D theCameraPosition, const int nearRadius, const int farRadius)
//{
//	for (int i = 0; i < xNumOfGrid; i++)
//	{
//		for (int j = 0; j < yNumOfGrid; j++)
//		{
//			// Render the grid and its objects
//			glPushMatrix();
//			//glTranslatef(xPos, yPos, zPos);
//			// Check the distance for this grid then decide the LOD to display
//			float distanceGrid2Camera = (theGrid[i * yNumOfGrid + j].GetCenterPoint() - theCameraPosition).GetMagnitude();
//			Vector3D nearCheck = ((theGrid[i * yNumOfGrid + j].GetCenterPoint() - theCameraPosition).normalized() * nearRadius) + theCameraPosition;
//			Vector3D farCheck = ((theGrid[i * yNumOfGrid + j].GetCenterPoint() - theCameraPosition).normalized() * farRadius) + theCameraPosition;
//
//			if (distanceGrid2Camera > farRadius)
//			{
//				theGrid[i * yNumOfGrid + j].Quality = 2;
//				if (CheckIfInGrid(farCheck, i, j, farRadius) && CheckGridCorners(theCameraPosition, i, j, farRadius))
//				{
//					theGrid[i * yNumOfGrid + j].Quality = 1;
//				}
//			}
//			else if (distanceGrid2Camera > nearRadius)
//			{
//				theGrid[i * yNumOfGrid + j].Quality = 1;
//				if (CheckIfInGrid(nearCheck, i, j, nearRadius) && CheckGridCorners(theCameraPosition, i, j, nearRadius))
//				{
//					theGrid[i * yNumOfGrid + j].Quality = 0;
//				}
//			}
//
//			theGrid[i * yNumOfGrid + j].RenderScene();
//			theGrid[i * yNumOfGrid + j].RenderObjects();
//			glPopMatrix();
//		}
//	}
//}

void CMap::RenderScene(const int x, const int y)
{
	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < yNumOfGrid; j++)
		{
			// Render the grid and its objects
			if (theGrid[i * yNumOfGrid + j].terrainType != 0) // If grid type is not blank
			{
				glPushMatrix();
				glTranslatef(xPos, yPos, 0);
				//glRotatef(90, 1, 0, 0);
				// Check the distance for this grid then decide the LOD to display
				/*float distanceGrid2Camera = (theGrid[i * yNumOfGrid + j].GetCenterPoint() - theCameraPosition).GetMagnitude();
				Vector3D nearCheck = ((theGrid[i * yNumOfGrid + j].GetCenterPoint() - theCameraPosition).normalized() * nearRadius) + theCameraPosition;
				Vector3D farCheck = ((theGrid[i * yNumOfGrid + j].GetCenterPoint() - theCameraPosition).normalized() * farRadius) + theCameraPosition;*/

				/*if (distanceGrid2Camera > farRadius)
				{
				theGrid[i * yNumOfGrid + j].Quality = 2;
				if (CheckIfInGrid(farCheck, i, j, farRadius) && CheckGridCorners(theCameraPosition, i, j, farRadius))
				{
				theGrid[i * yNumOfGrid + j].Quality = 1;
				}
				}
				else if (distanceGrid2Camera > nearRadius)
				{
				theGrid[i * yNumOfGrid + j].Quality = 1;
				if (CheckIfInGrid(nearCheck, i, j, nearRadius) && CheckGridCorners(theCameraPosition, i, j, nearRadius))
				{
				theGrid[i * yNumOfGrid + j].Quality = 0;
				}
				}*/

				theGrid[i * yNumOfGrid + j].CursorOnGrid(x, y);

				theGrid[i * yNumOfGrid + j].RenderScene();
				//theGrid[i * yNumOfGrid + j].RenderObjects();
				glPopMatrix();
			}
		}
	}
}

// Add a new object model
//void CSpatialPartition::AddObject(CObjectModel *theObjectModel)
//{
//	Vector3D theObjectPosition = theObjectModel->GetPosition();
//
//	int index_x = (int)(theObjectPosition.x / xSize);
//	int index_y = (int)(theObjectPosition.z / ySize);
//
//	if (((index_x >= 0) && (index_x < xNumOfGrid)) && ((index_y >= 0) && (index_y < yNumOfGrid)))
//	{
//		theGrid[index_x * yNumOfGrid + index_y].AddObject(theObjectModel);
//	}
//	else
//	{
//		// Do not add in since this object is outside of the grids
//	}
//}
//
//// Calculate the squared distance from camera to a grid's centerpoint
//float CSpatialPartition::CalculateDistanceSquare(Vector3D *theCameraPosition, const int xIndex, const int yIndex)
//{
//	float xDistance = (xIndex * xSize + (xSize / 2) + xPos) - theCameraPosition->x;
//	float yDistance = (yIndex * ySize + (ySize / 2) + zPos) - theCameraPosition->z;
//
//	return (float)(xDistance * xDistance + yDistance * yDistance);
//}
//
//Vector3D CSpatialPartition::CalculateDir(Vector3D *theCameraPosition, const int xIndex, const int yIndex)
//{
//	Vector3D dir = Vector3D((xIndex * xSize + (xSize / 2) + xPos) - theCameraPosition->x, 0, (yIndex * ySize + (ySize / 2) + zPos) - theCameraPosition->z);
//
//	return dir;
//}
//
//bool CSpatialPartition::CheckIfInGrid(Vector3D temp, const int xIndex, const int yIndex, int rad)
//{
//	if (temp.x > theGrid[xIndex * yNumOfGrid + yIndex].index_x * xSize && temp.x < ((theGrid[xIndex * yNumOfGrid + yIndex].index_x + 1) * xSize)
//		&& temp.z > theGrid[xIndex * yNumOfGrid + yIndex].index_y * ySize && temp.z < ((theGrid[xIndex * yNumOfGrid + yIndex].index_y + 1) * ySize))
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//bool CSpatialPartition::CheckGridCorners(Vector3D temp, const int xIndex, const int yIndex, int rad)
//{
//	Vector3D corner = theGrid[xIndex * yNumOfGrid + yIndex].GetBottomRight();
//	if ((corner - temp).GetMagnitude() < rad)
//	{
//		return true;
//	}
//
//	corner = theGrid[(xIndex + 1) * yNumOfGrid + yIndex].GetBottomRight();
//	if ((corner - temp).GetMagnitude() < rad)
//	{
//		return true;
//	}
//
//	corner = theGrid[xIndex * yNumOfGrid + yIndex].GetTopLeft();
//	if ((corner - temp).GetMagnitude() < rad)
//	{
//		return true;
//	}
//
//	corner = theGrid[(xIndex - 1)* yNumOfGrid + yIndex].GetTopLeft();
//	if ((corner - temp).GetMagnitude() < rad)
//	{
//		return true;
//	}
//	return false;
//}

int CMap::GetXNumOfGrid()
{
	return xNumOfGrid;
}

int CMap::GetYNumOfGrid()
{
	return yNumOfGrid;
}