#pragma once

#include <vector>
#include "TextureImage.h"
using namespace std;

class CGameState;

class CGameStateManager
{
public:
	void Init();
	void Init(const char* title, int width = 640, int height = 480,
		int bpp = 0, bool fullscreen = false);
	void Cleanup();

	void ChangeState(CGameState* state);
	void PushState(CGameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_bRunning; }
	void Quit() { m_bRunning = false; }
	static CGameStateManager* getInstance();

	void changeSize(int w, int h);
	void inputKey(int key, int x, int y);
	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void MouseMove(int x, int y);
	void MouseClick(int button, int state, int x, int y);

private:

	static CGameStateManager *s_pInstance;
	// the stack of states
	vector<CGameState*> StackOfStates;

	bool m_bRunning;
	bool m_bFullscreen;
};
