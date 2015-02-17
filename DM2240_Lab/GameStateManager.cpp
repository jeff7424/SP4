#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"


CGameStateManager * CGameStateManager::s_pInstance = NULL;


CGameStateManager* CGameStateManager::getInstance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new CGameStateManager();
	}

	return s_pInstance;
}

void CGameStateManager::Init(const char* title, int width, int height,
	int bpp, bool fullscreen)
{
	m_bFullscreen = fullscreen;
	m_bRunning = true;
}

void CGameStateManager::Init()
{
	StackOfStates.back()->Init();
}
void CGameStateManager::Cleanup()
{
	// cleanup the all states
	while (!StackOfStates.empty()) {
		StackOfStates.back()->Cleanup();
		StackOfStates.pop_back();
	}

	// switch back to windowed mode so other 
	// programs won't get accidentally resized
	if (m_bFullscreen) {
	}
}

void CGameStateManager::ChangeState(CGameState* state)
{
	// cleanup the current state
	if (!StackOfStates.empty()) {
		StackOfStates.back()->Cleanup();
		StackOfStates.pop_back();
	}

	// store and init the new state
	StackOfStates.push_back(state);
	StackOfStates.back()->Init();
}

void CGameStateManager::PushState(CGameState* state)
{
	// pause current state
	if (!StackOfStates.empty()) {
		StackOfStates.back()->Pause();
	}

	// store and init the new state
	StackOfStates.push_back(state);
	StackOfStates.back()->Init();
}

void CGameStateManager::PopState()
{
	// cleanup the current state
	if (!StackOfStates.empty()) {
		StackOfStates.back()->Cleanup();
		StackOfStates.pop_back();
	}

	// resume previous state
	if (!StackOfStates.empty()) {
		StackOfStates.back()->Resume();
	}
}


void CGameStateManager::HandleEvents()
{
	// let the state handle events
	StackOfStates.back()->HandleEvents(this);
}

void CGameStateManager::Update()
{
	// let the state update the theGSM
	StackOfStates.back()->Update(this);
}

void CGameStateManager::Draw()
{
	// let the state draw the screen
	StackOfStates.back()->Draw(this);
}

void CGameStateManager::changeSize(int w, int h) {
	StackOfStates.back()->changeSize(w, h);
}

void CGameStateManager::inputKey(int key, int x, int y) {
	StackOfStates.back()->inputKey(key, x, y);
}

void CGameStateManager::KeyboardDown(unsigned char key, int x, int y){
	StackOfStates.back()->KeyboardDown(key, x, y);
}

void CGameStateManager::KeyboardUp(unsigned char key, int x, int y){
	StackOfStates.back()->KeyboardUp(key, x, y);
}

void CGameStateManager::MouseMove(int x, int y){
	StackOfStates.back()->MouseMove(x, y);
}

void CGameStateManager::MouseClick(int button, int state, int x, int y){
	StackOfStates.back()->MouseClick(button, state, x, y);
}
