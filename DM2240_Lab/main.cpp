#include "main.h"
#include "GameStateManager.h"
#include "PlayState.h"
#include "MenuState.h"
#include "IntroState.h"
lua_State *luaState;

static int luaCreateWindow(lua_State *state);
static int luaRegisterCallbacks(lua_State *state);
static int luaStartMainLoop(lua_State *state);

void Init() {
	CGameStateManager::getInstance()->Init();
}

void changeSize(int w, int h) {
	CGameStateManager::getInstance()->changeSize(w,h);
}

void renderScene(void) {
	CGameStateManager::getInstance()->Draw();
}

void update(void) {
	CGameStateManager::getInstance()->Update();
}

void inputKey(int key, int x, int y) {
	CGameStateManager::getInstance()->inputKey(key, x, y);
}

void KeyboardDown(unsigned char key, int x, int y){
	CGameStateManager::getInstance()->KeyboardDown(key, x, y);
}

void KeyboardUp(unsigned char key, int x, int y){
	CGameStateManager::getInstance()->KeyboardUp(key, x, y);
}

void MouseMove(int x, int y){
	CGameStateManager::getInstance()->MouseMove(x, y);
}

void MouseClick(int button, int state, int x, int y){
	CGameStateManager::getInstance()->MouseClick(button, state, x, y);
}

void CleanUpUponExit(void)
{
	CGameStateManager::getInstance()->Cleanup();
}

void initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void createWindow(int width, int height, char *name, bool fullscreen)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	char text[256];
	sprintf(text, "%dx%d:16@60", width, height);
	glutGameModeString(text);
	if (fullscreen)
	{
		glutEnterGameMode();
	}
	glutCreateWindow(name);

	initGL();
}

void registerCallbacks()
{
	glutReshapeFunc(changeSize);
	glutIdleFunc(update);
	glutDisplayFunc(renderScene);

	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);

	glutMouseFunc(MouseClick);
	glutPassiveMotionFunc(MouseMove);
}

void startMainLoop()
{
	atexit(CleanUpUponExit);
	CGameStateManager::getInstance()->ChangeState(CIntroState::Instance());
	CGameStateManager::getInstance()->Init("Game State Test");
	glutMainLoop();
}

static int luaCreateWindow(lua_State *state)
{
	int n = lua_gettop(state);

	if (n < 4)
	{
		std::cout << "Error: createWindow(int,int,string,boolean) has insufficient parameters" << std::endl;
		lua_error(state);
		return 0;
	}

	// gets parameter
	int width = lua_tointeger(state, 1);
	int height = lua_tointeger(state, 2);
	char *name = (char*)lua_tostring(state, 3);
	bool full = lua_toboolean(state, 4);

	// call C++ create window function
	createWindow(width, height, name, full);

	return 1;
}

static int luaRegisterCallbacks(lua_State *state)
{
	registerCallbacks();

	return 1;
}

static int luaStartMainLoop(lua_State *state)
{
	startMainLoop();

	return 1;
}

int main(int argc, char **argv )
{
	glutInit(&argc, argv);

	luaState = lua_open();
	luaL_openlibs(luaState);

	lua_register(luaState, "createWindow", luaCreateWindow);
	lua_register(luaState, "registerCallbacks", luaRegisterCallbacks);
	lua_register(luaState, "startMainLoop", luaStartMainLoop);

	luaL_dofile(luaState, "main.lua");

	lua_close(luaState);

	return 0;
}