#pragma once
#include <vector>
#include "Vector3.h"
#include "TowerAtt.h"
#include "TextureImage.h"

using namespace std;

class CGrid
{
public:
	CGrid(void);
	~CGrid(void);
	// The x & y axis index of this grid in the spatial partition array
	float r;
	float g;
	float b;
	int index_x;
	int index_y;
	// The size of the grid in x and y axis
	int xSize;
	int ySize;
	int terrainType;
	bool CursorHit;
	// List of objects in this grid
	vector<Tower*> ListOfObjects;

	TextureImage texture;
	// Init
	void Init(const int index_x, const int index_y, const int xSize, const int ySize, int terrainType);
	// Add a new object to this grid
	void AddObject(Tower *tower);
	// Render Scene
	void RenderScene(void);
	// Render Objects
	// void RenderObjects();
	// Delete object from this grid
	void DeleteObjects(void);
	// Set color for the grid
	void SetColor();
	Vector3 GetCenterPoint();
	Vector3 GetTopLeft();
	Vector3 GetBottomRight();
	bool GetOccupied();
	void CursorOnGrid(const int x, const int y);
	void RenderStringOnScreen(float x, float y, const char* quote);
};

