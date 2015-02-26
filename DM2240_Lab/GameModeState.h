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
#include "irrKlang.h"
class CGameModeState : public CGameState
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

	static CGameModeState* Instance() {
		return &modeState;
	}

protected:
	CGameModeState() { }

private:
	ISoundEngine* buttons;
	AudioPlayer se;
	Button *CampaignButton;
	Button *SkirmishButton;
	Button *BackButton;
	bool isplaying;
	bool myKeys[255];
	TextureImage menu[1];
	TextureImage button[3];

	char* textures[5];

	Camera *theCam;
	theMouseInfo mouseInfo;

	bool LoadTGA(TextureImage *texture, char *filename);			// Loads A TGA File Into Memory
	static CGameModeState modeState;

	void *font_style;
	//  Draws a string at the specified coordinates.
	void printw(float x, float y, float z, char* format, ...);

	void RenderBackground(void);
	int LuaInit();
};