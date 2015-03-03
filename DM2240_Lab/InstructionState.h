#pragma once

#include "GameState.h"
#include "Camera.h"
#include "Mouse.h"
#include "Button.h"

class CInstructionState : public CGameState
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

	static CInstructionState* Instance() {
		return &InstructionState;
	}

protected:
	CInstructionState() { }

private:
	static CInstructionState InstructionState;

	int w;
	int h;

	Button *Back_Button;

	bool myKeys[255];
	TextureImage intro[1];

	Camera *theCam;
	theMouseInfo mouseInfo;

	bool LoadTGA(TextureImage *texture, char *filename);			// Loads A TGA File Into Memory
	void RenderIntro(void);
};
