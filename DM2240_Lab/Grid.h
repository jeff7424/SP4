#pragma once
#include <vector>
#include "ObjectModel.h"

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
	int Quality;
	// List of objects in this grid
	vector<CObjectModel*> ListOfObjects;

	// Init
	void Init(const int index_x, const int index_y, const int xSize, const int ySize);
	// Add a new object to this grid
	void AddObject(CObjectModel *theObject);
	// Render Scene
	void RenderScene(void);
	// Render Objects
	void RenderObjects();
	// Delete object from this grid
	void DeleteObjects(void);
	// Set color for the grid
	void SetColor();
	Vector3D GetCenterPoint();
	Vector3D GetTopLeft();
	Vector3D GetBottomRight();
};

