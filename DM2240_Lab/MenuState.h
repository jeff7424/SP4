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

class CMenuState : public CGameState
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

	static CMenuState* Instance() {
		return &theMenuState;
	}

protected:
	CMenuState() { }

private:

	Vector3 startButtonPos;
	Vector3 startButtonSize;
	Vector3 exitButtonPos;
	Vector3 exitButtonSize;

	bool myKeys[255];
	TextureImage menu[1];
	TextureImage button[2];

	char* textures[3];

	Camera *theCam;
	theMouseInfo mouseInfo;

	bool LoadTGA(TextureImage *texture, char *filename);			// Loads A TGA File Into Memory
	static CMenuState theMenuState;

	//GLvoid *font_style;
	void *font_style;
	//  Draws a string at the specified coordinates.
	void printw(float x, float y, float z, char* format, ...);

	void RenderStartButton();
	void RenderExitButton();
	void RenderMenu(void);
	int LuaInit();
};
