#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class CMap
{
public:
	CMap(void);
	~CMap(void);

	void Init( int theScreen_Height, int theScreen_Width, 
		       const int theMap_Height, const int theMap_Width, 
		       const int theTileSize);
	bool LoadMap(const string mapName);
	int getNumOfTiles_ScreenHeight(void);	// Get the number of tiles for height of the screen
	int getNumOfTiles_ScreenWidth(void);	// Get the number of tiles for width of the screen
	int getNumOfTiles_MapHeight(void);		// Get the number of tiles for height of the map
	int getNumOfTiles_MapWidth(void);		// Get the number of tiles for width of the map

	vector<vector<int> > theScreenMap;

private:
	int theScreen_Height;
	int theScreen_Width;
	int theMap_Height;
	int theMap_Width;
	int theNumOfTiles_ScreenHeight;
	int theNumOfTiles_ScreenWidth;
	int theNumOfTiles_MapHeight;
	int theNumOfTiles_MapWidth;
	int theTileSize;

	bool LoadFile(const string mapName);
};
