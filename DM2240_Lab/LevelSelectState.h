#pragma once

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "GameState.h"
#include <stdio.h>
#include <mmsystem.h>
#include "vector3.h"
#include "Camera.h"
#include "Mouse.h"
#include <string>
#include "Button.h"

class CLevelSelectState : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameStateManager* theGSM);
	void Update(CGameStateManager* theGSM);
	void Draw(CGameStateManager* theGSM);

	void changeSize(int w, int h);
	void inputKey(int key, int x, int y);
	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void MouseMove(int x, int y);
	void MouseClick(int button, int state, int x, int y);

	static CLevelSelectState* Instance() {
		return &levelSelectState;
	}

protected:
	CLevelSelectState() { }

private:
	int w; 
	int h;
	Button *BackButton;
	Button *Button_Level1;
	Button *Button_Level2;
	Button *Button_Level3;
	Button *Button_Level4;
	Button *Button_Level5;
	Button *Button_Level6;

	bool myKeys[255];
	TextureImage Map;

	char* textures[3];

	Camera *theCam;
	theMouseInfo mouseInfo;

	bool LoadTGA(TextureImage *texture, char *filename);			// Loads A TGA File Into Memory
	static CLevelSelectState levelSelectState;

	void *font_style;
	//  Draws a string at the specified coordinates.
	void printw(float x, float y, float z, char* format, ...);

	void RenderMenu(void);
	void CursorOnButton(int x, int y);

	int LuaInit();
};