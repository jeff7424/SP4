#include "Map.h"

CMap::CMap(void)
: theScreen_Height(0)
, theScreen_Width(0)
, theMap_Height(0)
, theMap_Width(0)
, theNumOfTiles_ScreenHeight(0)
, theNumOfTiles_ScreenWidth(0)
, theNumOfTiles_MapHeight(0)
, theNumOfTiles_MapWidth(0)
, theTileSize(0)
{
	theScreenMap.clear();
}

CMap::~CMap(void)
{
	theScreenMap.clear();
}

void CMap::Init(const int theScreen_Height, const int theScreen_Width, 
				const int theMap_Height, const int theMap_Width, 
				const int theTileSize)
{
	this->theScreen_Height = theScreen_Height;
	this->theScreen_Width = theScreen_Width;
	this->theMap_Height = theMap_Height;
	this->theMap_Width = theMap_Width;
	this->theTileSize = theTileSize;

	theNumOfTiles_ScreenHeight = (int) (theScreen_Height / theTileSize);
	theNumOfTiles_ScreenWidth = (int) (theScreen_Width / theTileSize);
	theNumOfTiles_MapHeight = (int) (theMap_Height / theTileSize);
	theNumOfTiles_MapWidth = (int) (theMap_Width / theTileSize);

	theScreenMap.resize(theNumOfTiles_MapHeight);
	for (int i = 0; i < theNumOfTiles_MapHeight; ++i)
		theScreenMap[i].resize(theNumOfTiles_MapWidth);
}

bool CMap::LoadMap(const string mapName)
{
	if (LoadFile(mapName) == true)
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}

	return false;
}

bool CMap::LoadFile(const string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;

	ifstream file(mapName.c_str());
	if(file.is_open())
	{
		int i = 0, k = 0;
		while(file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			// If this line is not a comment line, then process it
			if(!(aLineOfText.find("//") == NULL) && aLineOfText != "")
			{
				if (theLineCounter == 0)
				{
					// This is the first line of the map data file
					string token;
					istringstream iss(aLineOfText);
					while(getline(iss, token, ','))
					{
						// Count the number of columns
						theMaxNumOfColumns = atoi(token.c_str());
					}
				}
				else
				{
					int theColumnCounter = 0;

					string token;
					istringstream iss(aLineOfText);
					while(getline(iss, token, ','))
					{
						theScreenMap[theLineCounter][theColumnCounter++] = atoi(token.c_str());
					}
				}
			}

			theLineCounter++;
		}
	}
	return true;
}

// Get the number of tiles for height of the screen
int CMap::getNumOfTiles_ScreenHeight(void)
{
	return theNumOfTiles_ScreenHeight;
}

// Get the number of tiles for width of the screen
int CMap::getNumOfTiles_ScreenWidth(void)
{
	return theNumOfTiles_ScreenWidth;
}

// Get the number of tiles for height of the map
int CMap::getNumOfTiles_MapHeight(void)
{
	return theNumOfTiles_MapHeight;
}

// Get the number of tiles for width of the map
int CMap::getNumOfTiles_MapWidth(void)
{
	return theNumOfTiles_MapWidth;
}