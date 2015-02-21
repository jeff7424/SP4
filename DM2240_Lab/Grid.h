#pragma once
#include <vector>
#include "Vector3.h"
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
	bool Occupied;

	TextureImage texture;
	// Init
	void Init(const int index_x, const int index_y, const int xSize, const int ySize, int terrainType);
	void RenderScene(void);
	void SetColor();
	Vector3 GetCenterPoint();
	Vector3 GetTopLeft();
	Vector3 GetBottomRight();
	void SetOccupied(bool Occupied);
	bool GetOccupied();
	void CursorOnGrid(const int x, const int y);
	void RenderStringOnScreen(float x, float y, const char* quote);
	bool LoadTGA(TextureImage *texture, char *filename);
};

