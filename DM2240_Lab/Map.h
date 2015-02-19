#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "Grid.h"

class CMap
{
public:
	CMap(void);
	~CMap(void);

	bool LoadFile(const string filename, const int xPos, const int yPos, const int xSize, const int ySize);
	bool LoadMap(const string mapName, const int xPos, const int yPos, const int xSize, const int ySize);

	int Terrains[70];

	CGrid *theGrid;
	float xPos;
	float yPos;
	int xSize;
	int ySize;
	int xNumOfGrid;
	int yNumOfGrid;
	void Init(const int xPos, const int yPos, const int xSize, const int ySize, const int xNumOfGrid, const int yNumOfGrid, int terrainType);
	CGrid *GetGrid(const int xIndex, const int yIndex);
	//void RenderScene(Vector3 theCameraPosition, const int nearRadius, const int farRadius);
	void RenderScene(const int x, const int y);
	// Add a new object model
	//void AddObject(CObjectModel *theObjectModel);
	//// Calculate the squared distance from camera to a grid's centerpoint
	//float CalculateDistanceSquare(Vector3 *theCameraPosition, const int xIndex, const int yIndex);
	//Vector3 CalculateDir(Vector3 *theCameraPosition, const int xIndex, const int yIndex);
	//bool CheckIfInGrid(Vector3 temp, const int xIndex, const int yIndex, int rad);
	//bool CheckGridCorners(Vector3 temp, const int xIndex, const int yIndex, int rad);
	int GetXNumOfGrid();
	int GetYNumOfGrid();
};

