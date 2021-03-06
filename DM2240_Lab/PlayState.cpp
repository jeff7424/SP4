#include "GameState.h"
#include "GameStateManager.h"
#include "MiniGame.h"
#include "PlayState.h"
#include "MenuState.h"
#include <mmsystem.h>
#include "glext.h"
#include <lua.hpp>
#include <stdlib.h>
#include <time.h>

#pragma comment(linker, "/subsystem:\"console\" /entry:\"mainCRTStartup\"")
#pragma warning(disable:4996)

int CPlayState::progress = 1;

CPlayState CPlayState::thePlayState;
lua_State *L;

int a = 1;
int lane[6] = {0};
bool selection = false;
int RNGesus(void)
{
	return rand() % 3 + 1;
}

void CPlayState::Init(void)
{
	countcheck = 0;
	srand(time(NULL));
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	state = 0;
	selection = 1;
	heroAnimationCounter = 0;
	info = 0;
	spawntimer = 0.0f;
	ratio = 0;
	m_fps = 60.0f;
	m_speed = 1.0f;
	playmusic = true;
	pause = false;
	exitmenu = false;
	soundon = true;
	winscreen = false;
	losescreen = false;
	minigame = false;
	playvictory = false;
	for (int i = 0; i < 255; i++)
	{
		myKeys[i] = false;
	}
	Math::InitRNG();
	mouseInfo.lastX = 0;
	mouseInfo.lastY = 0;

	Cam = new Camera(Camera::LAND_CAM);
	Cam->SetPosition(0.0, 2.0, -5.0);
	Cam->SetDirection(0.0, 0.0, 1.0);

	// Player init
	player = new PlayerInfo();
	ifstream myReadFile;
	string reading;
	myReadFile.open("save/settings.txt");
	if (myReadFile.is_open())
	{
		getline(myReadFile, reading, '\n');
		audioplay = stoi(reading);
	}
	myReadFile.close();
	se = createIrrKlangDevice();
	if (audioplay == true)
	{
		playSound(a);
		if (winscreen != true)
		soundTypes(12);
	}
	// Enemy progress init
	tEnemyProgress = new CEnemyProgress();
	tEnemyProgress->SetPosX(0);
	tEnemyProgress->SetPosY(0);

	// Initialization map
	theMap = new CMap();

	backupTank = new Tank();

	Shield = new Powerup(Powerup::POWER_SHIELD);
	BaseHealth = new Powerup(Powerup::POWER_INCREASEBASEHEALTH);
	Firerate = new Powerup(Powerup::POWER_FIRERATEMULTIPLY);
	Damage = new Powerup(Powerup::POWER_DAMAGEMULTIPLY);
	Backup_Tank = new Powerup(Powerup::POWER_TANKBACKUP);

	theNumOfTiles_Height = theMap->GetYNumOfGrid();
	theNumOfTiles_Width = theMap->GetXNumOfGrid();

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	LoadTGA(&BackgroundTexture[0], "bin/textures/game_background.tga");
	LoadTGA(&Cursor[0], "bin/tower/Unit1_Soldier.tga");
	LoadTGA(&Cursor[1], "bin/tower/Unit2_Tank.tga");
	LoadTGA(&Cursor[2], "bin/tower/Unit3_RPG.tga");
	LoadTGA(&Cursor[3], "bin/tower/Unit4_Sniper.tga");
	LoadTGA(&Cursor[4], "bin/tower/Unit5_Mine.tga");
	LoadTGA(&Cursor[5], "bin/tower/Unit6_Barricade.tga");
	/*LoadTGA(&CreepTexture[0], "bin/textures/redpoker.tga");
	LoadTGA(&CreepTexture[1], "bin/textures/redcard.tga");
	LoadTGA(&CreepTexture[2], "bin/textures/ahlong.tga");*/

	LoadTGA(&PauseMenu, "bin/ui/pausemenu/pausemenu.tga");
	LoadTGA(&ExitMenu, "bin/ui/pausemenu/exitmenu.tga");
	LoadTGA(&WinScreenTexture, "bin/textures/winscreen.tga");
	LoadTGA(&LoseScreenTexture, "bin/textures/losescreen2.tga");

	// Pause button
	Button_Pause = new Button("bin/ui/hud/button_pause.tga", 896, 48, 32, 32);

	// Buttons for powers
	Power_Shield = new Button("bin/ui/hud/button_powershield.tga", 608, 624, 36, 36);
	Power_BaseHealth = new Button("bin/ui/hud/button_powerhealth.tga", 680, 624, 36, 36);
	Power_Firerate = new Button("bin/ui/hud/button_powerspeed.tga", 752, 624, 36, 36);
	Power_Damage = new Button("bin/ui/hud/button_powerdmg.tga", 824, 624, 36, 36);
	Power_BackupTank = new Button("bin/ui/hud/button_powertank.tga", 896, 624, 36, 36);

	// Buttons for selecting units
	Unit_Infantry = new Button("bin/ui/hud/button_unit1.tga", 48, 624, 36, 36);
	Unit_Tank = new Button("bin/ui/hud/button_unit2.tga", 120, 624, 36, 36);
	Unit_Heavy = new Button("bin/ui/hud/button_unit3.tga", 192, 624, 36, 36);
	Unit_Sniper = new Button("bin/ui/hud/button_unit4.tga", 264, 624, 36, 36);
	Unit_Mine = new Button("bin/ui/hud/button_unit5.tga", 336, 624, 36, 36);
	Unit_Barricade = new Button("bin/ui/hud/button_unit6.tga", 408, 624, 36, 36);

	//For Win Lose Menu
	WinLose_MainMenu = new Button("bin/ui/hud/button_mainmenu.tga", 832, 594, 108, 28);
	WinLose_RestartLevel = new Button("bin/ui/hud/button_restart.tga", 832, 494, 108, 28);
	WinLose_NextLevel = new Button("bin/ui/hud/button_nextlevel.tga", 832, 194, 108, 28);

	//For Mini Game
	WinLose_MiniGame = new Button("bin/ui/hud/button_minigame.tga", 832, 294, 108, 28);

	// Shops
	Bonus_Attack = new Button("bin/ui/hud/button_attackbonus.tga", 370, 300, 48, 48);
	Bonus_Armour = new Button("bin/ui/hud/button_armourbonus.tga", 470, 300, 48, 48);
	Bonus_Dollar = new Button("bin/ui/hud/button_dollarbonus.tga", 570, 300, 48, 48);
	
	Bonus_PShd = new Button("bin/ui/hud/button_powershield.tga", 270, 400, 48, 48);
	Bonus_PHealth = new Button("bin/ui/hud/button_powerhealth.tga", 370, 400, 48, 48);
	Bonus_PFirerate = new Button("bin/ui/hud/button_powerspeed.tga", 470, 400, 48, 48);
	Bonus_PDamage = new Button("bin/ui/hud/button_powerdmg.tga", 570, 400, 48, 48);
	Bonus_PTank = new Button("bin/ui/hud/button_powertank.tga", 670, 400, 48, 48);

	Shop_BG	     = new BG("bin/ui/hud/ShopBG32.tga", 470, 325, 251, 127);

	Bonus_MultAttack = 1;
	Bonus_MultArmour = 1;
	Bonus_MultDollar = 1;

	// For Pause Menu
	PauseMenu_Resume = new Button("bin/ui/pausemenu/button_resume.tga", 480, 250, 128, 32);
	PauseMenu_Restart = new Button("bin/ui/pausemenu/button_restart.tga", 480, 350, 128, 32);
	PauseMenu_Exit = new Button("bin/ui/pausemenu/button_quit.tga", 480, 450, 128, 32);
	ExitMenu_Yes = new Button("bin/ui/pausemenu/button_yes.tga", 480, 350, 128, 32);
	ExitMenu_No = new Button("bin/ui/pausemenu/button_no.tga", 480, 450, 128, 32);

	//theMiniGame = new CMiniGame();

	// Load the attributes through text file
	LoadAtt();
	if (!load)
	{	
		loadlevel();
		LoadSpawn();
	}
	else
	{
		Load();
	}
}

void CPlayState::Cleanup()
{
	if (tEnemyProgress != NULL)
	{
		delete tEnemyProgress;
		tEnemyProgress = NULL;
		free(tEnemyProgress);
	}

	if (player != NULL)
	{
		delete player;
		player = NULL;
		free(player);
	}

	// Power ups deconstruct
	if (Shield != NULL) 
	{ 
		delete Shield; 
		Shield = NULL; 
		free(Shield); 
	}

	if (BaseHealth != NULL) 
	{ 
		delete BaseHealth; 
		BaseHealth = NULL; 
		free(BaseHealth); 
	}

	if (Firerate != NULL) 
	{ 
		delete Firerate; 
		Firerate = NULL; 
		free(Firerate); 
	}
	
	if (Damage != NULL) 
	{ 
		delete Damage; 
		Damage = NULL; 
		free(Damage); 
	}
	
	if (Backup_Tank != NULL) 
	{ 
		delete Backup_Tank; 
		Backup_Tank = NULL; 
		free(Backup_Tank); 
	}

	// Buttons deconstruct
	if (Button_Pause != NULL) 
	{ 
		delete Button_Pause; 
		Button_Pause = NULL; 
		free(Button_Pause); 
	}

	if (PauseMenu_Resume != NULL) 
	{ 
		delete PauseMenu_Resume; 
		PauseMenu_Resume = NULL; 
		free(PauseMenu_Resume); 
	}
	
	if (PauseMenu_Restart != NULL) 
	{ 
		delete PauseMenu_Restart; 
		PauseMenu_Restart = NULL; 
		free(PauseMenu_Restart); 
	}
	
	if (PauseMenu_Exit != NULL) 
	{ 
		delete PauseMenu_Exit; 
		PauseMenu_Exit = NULL; 
		free(PauseMenu_Exit); 
	}
	
	if (ExitMenu_Yes != NULL) 
	{ 
		delete ExitMenu_Yes; 
		ExitMenu_Yes = NULL; 
		free(ExitMenu_Yes); 
	}

	if (ExitMenu_No != NULL)
	{
		delete ExitMenu_No;
		ExitMenu_No = NULL;
		free(ExitMenu_No);
	}

	if (Power_Shield != NULL)
	{
		delete Power_Shield;
		Power_Shield = NULL;
		free(Power_Shield);
	}
	if (Power_BaseHealth != NULL)
	{
		delete Power_BaseHealth;
		Power_BaseHealth = NULL;
		free(Power_BaseHealth);
	}
	if (Power_Firerate != NULL)
	{
		delete Power_Firerate;
		Power_Firerate = NULL;
		free(Power_Firerate);
	}
	if (Power_Damage != NULL)
	{
		delete Power_Damage;
		Power_Damage = NULL;
		free(Power_Damage);
	}
	if (Power_BackupTank != NULL)
	{
		delete Power_BackupTank;
		Power_BackupTank = NULL;
		free(Power_BackupTank);
	}

	if (Unit_Infantry != NULL)
	{
		delete Unit_Infantry;
		Unit_Infantry = NULL;
		free(Unit_Infantry);
	}
	if (Unit_Tank != NULL)
	{
		delete Unit_Tank;
		Unit_Tank = NULL;
		free(Unit_Tank);
	}
	if (Unit_Heavy != NULL)
	{
		delete Unit_Heavy;
		Unit_Heavy = NULL;
		free(Unit_Heavy);
	}
	if (Unit_Sniper != NULL)
	{
		delete Unit_Sniper;
		Unit_Sniper = NULL;
		free(Unit_Sniper);
	}
	if (Unit_Mine != NULL)
	{
		delete Unit_Mine;
		Unit_Mine = NULL;
		free(Unit_Mine);
	}
	if (Unit_Barricade != NULL)
	{
		delete Unit_Barricade;
		Unit_Barricade = NULL;
		free(Unit_Barricade);
	}
	if (Bonus_Attack != NULL)
	{
		delete Bonus_Attack;
		Bonus_Attack = NULL;
		free(Bonus_Attack);
	}
	if (Bonus_Armour != NULL)
	{
		delete Bonus_Armour;
		Bonus_Armour = NULL;
		free(Bonus_Armour);
	}
	if (Bonus_Dollar != NULL)
	{
		delete Bonus_Dollar;
		Bonus_Dollar = NULL;
		free(Bonus_Dollar);
	}

	/*while (bulletList.size() > 0)
	{
		Bullet *bullet = bulletList.back();
		delete bullet;
		bullet = NULL;
		bulletList.pop_back();
		free(bullet);
	}

	while (enemyList.size() > 0)
	{
		Enemy *creep = enemyList.back();
		delete creep;
		creep = NULL;
		enemyList.pop_back();
		free(creep);
	}

	while (towerList.size() > 0)
	{
		Tower *tower = towerList.back();
		delete tower;
		tower = NULL;
		towerList.pop_back();
		free(tower);
	}

	while (towerClone.size() > 0)
	{
		Tower *clone = towerClone.back();
		delete clone;
		clone = NULL;
		towerClone.pop_back();
		free(clone);
	}

	while (enemyClone.size() > 0)
	{
		Enemy *clone = enemyClone.back();
		delete clone;
		clone = NULL;
		enemyClone.pop_back();
		free(clone);
	}

	while (spawnList.size() > 0)
	{
		Spawn *spawn = spawnList.back();
		delete spawn;
		spawn = NULL;
		spawnList.pop_back();
		free(spawn);
	}*/
	towerList.clear();
	towerClone.clear();
	enemyList.clear();
	enemyClone.clear();
	bulletList.clear();
	spawnList.clear();
}

void CPlayState::Pause()
{
}

void CPlayState::Resume()
{
}

void CPlayState::HandleEvents(CGameStateManager* theGSM)
{
	/*int m_iUserChoice = -1;

	do {
	cout << "CPlayState: Choose one <0> Go to Menu State : " ;
	cin >> m_iUserChoice;
	cin.get();

	switch (m_iUserChoice) {
	case 0:
	theGSM->ChangeState( CMenuState::Instance() );
	break;
	default:
	cout << "Invalid choice!\n";
	m_iUserChoice = -1;
	break;
	}
	} while (m_iUserChoice == -1);


	}*/

}

void CPlayState::Update(CGameStateManager* theGSM)
{
	static int frame = 0;
	static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	++frame;
	int time = glutGet(GLUT_ELAPSED_TIME);
	float dt = ((time - lastTime) / 1000.f);
	lastTime = time;

	static int lastFPSTime = glutGet(GLUT_ELAPSED_TIME);
	if ((time - lastFPSTime) > 1000)
	{
		m_fps = (frame * 1000.f / (time - lastFPSTime));
		lastFPSTime = time;
		frame = 0;
	}

	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);

	if (!pause && !winscreen && !losescreen)
	{
		// timer for enemy to spawn
		spawntimer += dt;

		// power ups update (duration & cooldown)
		Shield->Update(dt);
		BaseHealth->Update(dt);
		Firerate->Update(dt);
		Damage->Update(dt);
		Backup_Tank->Update(dt);

		// Spawn enemies
		UpdateSpawn();

		if (Backup_Tank->GetActive())
		{
			backupTank->SetActive(true);
		}
		else
		{
			backupTank->SetActive(false);
		}
		backupTank->Update(dt);
		Shield->Update(dt);
		BaseHealth->Update(dt);
		Firerate->Update(dt);
		Damage->Update(dt);

		for (std::vector<Enemy*>::iterator itr = enemyList.begin(); itr != enemyList.end(); ++itr)
		{
			if ((*itr)->GetActive() == false)
			{
				delete *itr;
				enemyList.erase(itr);
				break;
			}
		}

		for (std::vector<Bullet*>::iterator itr2 = bulletList.begin(); itr2 != bulletList.end(); ++itr2)
		{
			if ((*itr2)->GetActive() == false)
			{
				delete *itr2;
				bulletList.erase(itr2);
				break;
			}
		}

		for (std::vector<Tower*>::iterator itr3 = towerList.begin(); itr3 != towerList.end(); ++itr3)
		{
			if ((*itr3)->GetActive() == false)
			{
				delete *itr3;
				towerList.erase(itr3);
				break;
			}
		}

		for (unsigned int it = 0; it < enemyList.size(); ++it)
		{
			Enemy* enemy = enemyList[it];
			if (enemy->GetActive())
			{
				backupTank->GetTarget(enemy);
			}
		}

		// Tower update
		for (unsigned int it = 0; it != towerList.size(); ++it)
		{
			Tower* tower = towerList[it];
			if (tower->GetActive())
			{
				tower->Update(dt, Firerate, Damage);
				tower->GetTarget(enemyList);
				if (tower->state == Tower::STATE_ATTACK)
				{
					switch (tower->type)
					{
						case Tower::TOWER_SLOW:
							soundTypes(6);
						break;
						case Tower::TOWER_NORMAL:
							soundTypes(7);
							soundTypes(8);
						break;
						case Tower::TOWER_SHOCK:
							soundTypes(9);
						break;
						case Tower::TOWER_CANNON:
							soundTypes(10);
							break;
						case Tower::TOWER_MINE:
							soundTypes(4);
							tower->ReturnTarget()->SetHealth(tower->ReturnTarget()->GetHealth() - 80);
							if (tower->ReturnTarget()->GetHealth() <= 0)
							{
								tower->ReturnTarget()->SetActive(false);
							}
							tower->SetHealth(0);
							tower->SetActive(false);
							tEnemyProgress->SetEnemyCounter(tEnemyProgress->GetEnemyCounter() - 1);
							Deathsounds();
							int x = (int)((tower->GetPos().x / TILE_SIZE) - 0.5f);
							int y = (int)((tower->GetPos().y / TILE_SIZE) - 0.5f);
							theMap->GetGrid(x, y)->SetOccupied(false);
							//soundTypes(creep->type, true);
							switch (tower->ReturnTarget()->type)
							{
							case Enemy::ENEMY_1:
								player->SetGold(player->GetGold() + (Enemy::NME_Y1)*Bonus_MultDollar);
								break;

							case Enemy::ENEMY_2:
								player->SetGold(player->GetGold() + (Enemy::NME_Y2)*Bonus_MultDollar);
								break;

							case Enemy::ENEMY_3:
								player->SetGold(player->GetGold() + (Enemy::NME_Y3)*Bonus_MultDollar);
								player->SetBonus(player->GetBonus() + 1);
								break;
							}
							break;
					}
				}
			}
		}

		// Deactivate out of bounds objects
		for (std::vector<Bullet *>::iterator it = bulletList.begin(); it != bulletList.end(); ++it)
		{
			Bullet *bullet = *it;
			if (bullet->GetActive() && bullet->type != Bullet::GO_BOMBBULLET)
			{
				bullet->Update(dt);
				if (bullet->GetPos().x >= SCREEN_WIDTH || bullet->GetPos().x < 0)
				{
					bullet->SetActive(false);
				}
			}
			else
			{
				if (bullet->type == Bullet::GO_CANNONBULLET || Bullet::GO_SHOCKBULLET)
					soundTypes(4);
				else
				soundTypes(11);
			}
		}

		theEnemy->Update(enemyList, towerList, dt, laneCheck());

		// Handle enemies which reaches the base
		for (std::vector<Enemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
		{
			Enemy *creep = *it;
			if (creep->GetPos().x <= 0)	// Creep leaves the left side of the screen and inflicts DAMAGE ON BASE!
			{
				switch (creep->type)
				{
				case Enemy::ENEMY_1:
					if (player->GetShield() > 0)
					{
						player->SetShield(player->GetShield()-10);
						if (player->GetShield() < 0)
							player->SetShield(0);
					}
					else
						player->SetHealth(player->GetHealth()-10);
					break;

				case Enemy::ENEMY_2:
					if (player->GetShield() > 0)
					{
						player->SetShield(player->GetShield()-20);
						if (player->GetShield() < 0)
							player->SetShield(0);
					}
					else
						player->SetHealth((player->GetHealth())-20);
					break;

				case Enemy::ENEMY_3:
					if (player->GetShield() > 0)
					{
						player->SetShield(player->GetShield()-30);
						if (player->GetShield() < 0)
							player->SetShield(0);
					}
					else
						player->SetHealth((player->GetHealth())-30);
					break;
				}
				creep->SetActive(false);
				tEnemyProgress->SetEnemyCounter(tEnemyProgress->GetEnemyCounter() - 1);
				player->SetHealth(player->GetHealth() - creep->GetDamage());
				break;
			}
		}
		
		//for (unsigned int i = 0; i < bulletList.size(); ++i)
		//{
		//	if (bulletList[i]->GetActive() && bulletList[i]->type != Bullet::GO_BOMBBULLET)
		//	{
		//		bulletList[i]->SetPos(Vector3(bulletList[i]->GetPos().x + bulletList[i]->GetSpeed() * dt, bulletList[i]->GetPos().y, 0));
		//	}
		//}

		// Collision updates and unit triggers
		Update(dt);
	}
}

int CPlayState::laneCheck()
{
	int k = 1; 
    int min = lane[1];       // start with max = first element
	
     for(int i = 1; i < 6; i++)
     {
          if(lane[i] < min)
          {
			  min = lane[i];
			  k = i;
		  }

			cout << lane[i] << " ";
     }
	 cout << " { " << k << " } " << endl;
	 //cout << " [ " << min << " ] " << endl;
     return k;                // return strongest lane 
}

void CPlayState::Draw(CGameStateManager* theGSM) {
	//int w = glutGet(GLUT_WINDOW_WIDTH);
	//int h = glutGet(GLUT_WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	Cam->Update();

	// HUD
	Cam->SetHUD(true);

	RenderBackground();
	RenderTileMap();

	backupTank->Render();

	// Render towers
	for (std::vector<Tower *>::iterator it = towerList.begin(); it != towerList.end(); ++it)
	{
		Tower *tower = *it;
		if (tower->GetActive() == true)
		{
			tower->Render();
		}
	}

	int time = glutGet(GLUT_ELAPSED_TIME);
	static int ctime = glutGet(GLUT_ELAPSED_TIME);
		if (time - ctime > 100) // the more it is the slower it becomes
		{
			heroAnimationCounter--;
			if (heroAnimationCounter == 0)
			{
				heroAnimationCounter = 6;
			}
			ctime = time;
		}

	// Render enemies
	for (std::vector<Enemy *>::iterator it2 = enemyList.begin(); it2 != enemyList.end(); ++it2)
	{
		Enemy *creep = *it2;
		if (creep->GetActive() == true)
		{
			creep->DrawEnemy(heroAnimationCounter);
		}
	}

	// Render bullets
	for (std::vector<Bullet *>::iterator it3 = bulletList.begin(); it3 != bulletList.end(); ++it3)
	{
		Bullet *bullet = *it3;
		if (bullet->GetActive() == true)
		{
			bullet->Render(heroAnimationCounter);
		}
	}

	RenderHUD();

	Cam->SetHUD(false);

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void CPlayState::changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	//if (h == 0)
	//	h = 1;

	//ratio = (float)(1.0f* w / h);

	//// Reset the coordinate system before modifying
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//// Set the viewport to be the entire window
	//glViewport(0, 0, w, h);

	//// Set the correct perspective.
	//gluPerspective(45, ratio, 1, 1000);
	//glMatrixMode(GL_MODELVIEW);

	if (h == 0)
		h = 1;

	ratio = (float)w / h;

	/*WX = w;
	WY = h;*/

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
	//gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

	//float ar = 65.0f / 46.0f;

	//int viewW = w;
	//int viewH = w / ar;
	//if (viewH > h)
	//{
	//	viewH = h;
	//	viewW = h * ar;
	//}

	//int vpX = (w - viewW) / 2;
	//int vpY = (h - viewH) / 2;

	//// Reset the coordinate system before modifying
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//// Set the viewport to be the entire window
	//glViewport(vpX, vpY, viewW, viewH);

	//// Set the correct perspective.
	//gluPerspective(45, ar, 1, 1000);
	//glMatrixMode(GL_MODELVIEW);

	//this->w = viewW / ar;
	//this->h = viewH;
}

void CPlayState::inputKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}
}

void CPlayState::KeyboardDown(unsigned char key, int x, int y){

	myKeys[key] = true;
	switch (key)
	{
	/*case 'w':
		theMiniGame->SetPos(Vector3(theMiniGame->GetPos().x, theMiniGame->GetPos().y - 20, 0));
		break;
	case 'a':
		theMiniGame->SetPos(Vector3(theMiniGame->GetPos().x - 20, theMiniGame->GetPos().y, 0));
		break;
	case 's':
		theMiniGame->SetPos(Vector3(theMiniGame->GetPos().x, theMiniGame->GetPos().y + 20, 0));
		break;
	case 'd':
		theMiniGame->SetPos(Vector3(theMiniGame->GetPos().x + 20, theMiniGame->GetPos().y, 0));
		break;*/

	case '1':
		selection = 1;
		break;
	case '2':
		selection = 2;
		break;
	case '3':
		selection = 3;
		break;
	case '4':
		selection = 4;
		break;
	case '5':
		selection = 5;
		break;
	case '6':
		selection = 6;
		break;
	case '7':

		break;
	case '-':
		a++;
		if (a > 3)
		{
			a = 1;
		}
		playSound(a);
		break;
	case '_':
		sound.decreaseVolume();
		break;
	case '=':
	case '+':
		sound.increaseVolume();
		break;
	case ' ':
		if (playmusic == true)
		{
			sound.pause();
			playmusic = false;
		}
		else
		{
			if (audioplay == true)
			{
				sound.resume();
			}
			playmusic = true;
		}
		break;
	case 27:
		exit(0);
		break;
	}
}

void CPlayState::KeyboardUp(unsigned char key, int x, int y){

	myKeys[key] = false;
}

void CPlayState::MouseMove(int x, int y) {
	mouseInfo.lastX = (int)((float)x / w * SCREEN_WIDTH);
	mouseInfo.lastY = (int)((float)y / h * SCREEN_HEIGHT);
	/*int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);*/
	//mouseInfo.mLButtonUp = !!state;

	//moverlevel1(x, y);

	
	/*if (minigame == true)
	{
		theMiniGame->SetPos(Vector3(mouseInfo.lastX, mouseInfo.lastY, 0));
	}*/

	Button_Pause->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);

	PauseMenu_Resume->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
	PauseMenu_Restart->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
	PauseMenu_Exit->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);

	if (exitmenu)
	{
		ExitMenu_Yes->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		ExitMenu_No->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
	}

	if (winscreen == true)
	{
		//For Win Screen
		WinLose_MainMenu->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		WinLose_RestartLevel->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		WinLose_NextLevel->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
//		WinLose_Shop->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		WinLose_MiniGame->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		
		Bonus_Attack->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Bonus_Armour->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Bonus_Dollar->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);

		Bonus_PShd->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Bonus_PHealth->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Bonus_PFirerate->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Bonus_PDamage->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Bonus_PTank->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		
		if (Bonus_Attack->GetIsHover())
		{
			info = -1;
		}
		if (Bonus_Armour->GetIsHover())
		{
			info = -2;
		}
		if (Bonus_Dollar->GetIsHover())
		{
			info = -3;
		}
	}

	else if (winscreen == true && progress > 5)
	{
		WinLose_MainMenu->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		WinLose_RestartLevel->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
	}

	else if (losescreen == true)
	{
		//For Lose Screens
		WinLose_MainMenu->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		WinLose_RestartLevel->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
//		WinLose_Shop->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
	}

	if (!pause)
	{
		Power_Shield->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Power_BaseHealth->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Power_Firerate->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Power_Damage->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Power_BackupTank->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);

		Unit_Infantry->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Unit_Tank->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Unit_Heavy->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Unit_Sniper->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Unit_Mine->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);
		Unit_Barricade->SetIsHover(mouseInfo.lastX, mouseInfo.lastY);

		if (Unit_Infantry->GetIsHover())
		{
			info = 1;
		}
		else if (Unit_Tank->GetIsHover())
		{
			info = 2;
		}
		else if (Unit_Heavy->GetIsHover())
		{
			info = 3;
		}
		else if (Unit_Sniper->GetIsHover())
		{
			info = 4;
		}
		else if (Unit_Mine->GetIsHover())
		{
			info = 5;
		}
		else if (Unit_Barricade->GetIsHover())
		{
			info = 6;
		}
		else
		{
			info = 0;
		}
	}
}

void CPlayState::MouseClick(int button, int state, int x, int y) {
	mouseInfo.lastX = (int)((float)x / w * SCREEN_WIDTH);
	mouseInfo.lastY = (int)((float)y / h * SCREEN_HEIGHT);

	switch (button) {

	case GLUT_LEFT_BUTTON:
		if (state == 0)
		{
			mouseInfo.mLButtonUp = false;
			mclicklevel1(x, y);
		}
		else
			mouseInfo.mLButtonUp = true;
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == 0)
			mouseInfo.mRButtonUp = false;
		else
			mouseInfo.mRButtonUp = true;
		break;
	}
}

bool CPlayState::LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
{
	GLubyte		TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type = GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	/*
	// NEW version for opening a file
	FILE *file = NULL;
	errno_t err;

	// Open for read (will fail if file "crt_fopen_s.c" does not exist)
	err  = fopen_s( &file, filename, "rb" );
	if( err == 0 )
	{
	cout << "The file " << filename << " was opened" << endl;
	}
	else
	{
	cout << "The file " << filename << " was UNABLE to be opened!" << endl;
	}
	*/

	FILE *file = fopen(filename, "rb");						// Open The TGA File

	if (file == NULL ||										// Does File Even Exist?
		fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||	// Does The Header Match What We Want?
		fread(header, 1, sizeof(header), file) != sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
			return false;									// Return False
		else
		{
			fclose(file);									// If Anything Failed, Close The File
			return false;									// Return False
		}
	}

	texture->width = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)

	if (texture->width <= 0 ||								// Is The Width Less Than Or Equal To Zero
		texture->height <= 0 ||								// Is The Height Less Than Or Equal To Zero
		(header[4] != 24 && header[4] != 32))					// Is The TGA 24 or 32 Bit?
	{
		fclose(file);										// If Anything Failed, Close The File
		return false;										// Return False
	}

	texture->bpp = header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel = texture->bpp / 8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize = texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData = (GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if (texture->imageData == NULL ||							// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file) != imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if (texture->imageData != NULL)						// Was Image Data Loaded
			free(texture->imageData);						// If So, Release The Image Data

		fclose(file);										// Close The File
		return false;										// Return False
	}

	for (GLuint i = 0; i<int(imageSize); i += bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = texture->imageData[i];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose(file);											// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);					// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);			// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered

	if (texture[0].bpp == 24)									// Was The TGA 24 Bits
	{
		type = GL_RGB;										// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;											// Texture Building Went Ok, Return True
}

void CPlayState::RenderTileMap(void) 
{
	theMap->RenderScene(mouseInfo.lastX, mouseInfo.lastY);
}

void CPlayState::mclicklevel1(int x, int y)
{
	if (tEnemyProgress->GetEnemyCounter() > 0)
	{
		int X = (float)x / w * theMap->GetXNumOfGrid();
		int Y = (float)y / h * theMap->GetYNumOfGrid();

		Tower *tower = NULL;
		if (tower != NULL)
		{
			delete tower;
			tower = NULL;
		}
		if (Button_Pause->GetIsHover())
		{
			pause = !pause;
			exitmenu = false;
			std::cout << pause << std::endl;
		}
	
		if (pause)
		{
			if (!exitmenu)
			{
				if (PauseMenu_Resume->GetIsHover())
				{
					pause = false;
					std::cout << pause << std::endl;
				}
				else if (PauseMenu_Restart->GetIsHover())
				{
					pause = false;
					cout << " Restart Level!" << endl;
					spawntimer = 0.0f;
					player->SetHealth(player->GetMaxHealth());
					player->SetGold(1000);
					clearmap();
					loadlevel();
					tEnemyProgress->SetEnemyCounter(0);
					LoadSpawn();
				}
				else if (PauseMenu_Exit->GetIsHover())
				{
					exitmenu = true;
				}
			}
			
			else if (exitmenu)
			{
				if (ExitMenu_Yes->GetIsHover())
				{
					sound.stop();
					Save();
					pause = false;
					exitmenu = false;
					CGameStateManager::getInstance()->ChangeState(CMenuState::Instance());
				}
				else if (ExitMenu_No->GetIsHover())
				{
					exitmenu = false;
				}
			}
		}
		else if (!pause && !winscreen && !losescreen)
		{
			if (theMap->GetGrid(X, Y)->CursorHit == true)
			{
				// if it is a terrain
				if (theMap->GetGrid(X, Y)->terrainType != 0)
				{
					if (!theMap->GetGrid(X, Y)->GetOccupied())
					{
						if (tower = FetchTower())
						{
							//tower->type = static_cast<Tower::TOWER_TYPE>(selection);
							tower->SetAtt(towerClone[selection - 1]->GetFireRate(), towerClone[selection - 1]->GetCost(),
								towerClone[selection - 1]->GetDamage(), towerClone[selection - 1]->GetRange(), towerClone[selection - 1]->GetHealth());
							tower->SetMaxHealth(tower->GetHealth());
							if (player->GetGold() >= tower->GetCost())
							{
								if (tower->type == Tower::TOWER_SHOCK)
								{
									soundTypes(20);
									soundTypes(17);
								}
								else if (tower->type == Tower::TOWER_SLOW)
								{
									soundTypes(20);
									soundTypes(18);
								}
								else if (tower->type == Tower::TOWER_BARRICADE)
								{
									soundTypes(20);
									soundTypes(19);
								}
								else if (tower->type == Tower::TOWER_MINE)
								{
									soundTypes(20);
									soundTypes(21);
								}
								else if (tower->type == Tower::TOWER_CANNON)
								{
									soundTypes(20);
									soundTypes(22);
								}
								else
								{
									soundTypes(20);
									soundTypes(14);
								}
								lane[Y] += 1;
								tower->SetActive(true);
								tower->SetLevel(1);
								tower->SetPos(Vector3(theMap->GetGrid(X, Y)->GetCenterPoint().x, theMap->GetGrid(X, Y)->GetCenterPoint().y, 0));
								theMap->GetGrid(X, Y)->SetOccupied(true);
								player->SetGold(player->GetGold() - tower->GetCost()); 
								towerList.push_back(tower);
							}
							else
							{
								soundTypes(15);
								tower->SetActive(false);
							}
						}
					}
				}
			}
			if (Power_Shield->GetIsHover())
			{
				if (player->GetQtyShield() > 0)
				{
					if (Shield->GetReady())
					{
						soundTypes(24);
						Shield->SetActive(true);
						player->SetMaxShield(player->GetShield() + 50);
						player->SetShield(player->GetShield() + 50);
						player->SetQtyShield(player->GetQtyShield() - 1);
					}
				}
				else
				{
					soundTypes(15);
				}
			}
			else if (Power_BaseHealth->GetIsHover())
			{
				if (player->GetQtyBaseHealth() > 0)
				{
					if (BaseHealth->GetReady())
					{
						if (player->GetHealth() < player->GetMaxHealth())
						{
							soundTypes(24);
							BaseHealth->SetActive(true);
							player->SetHealth(player->GetHealth() + 50);
							player->SetQtyBaseHealth(player->GetQtyBaseHealth() - 1);
							if (player->GetHealth() >= 100)
								player->SetHealth(100);
						}
					}
				}
				else
				{
					soundTypes(15);
				}
			}
			else if (Power_Firerate->GetIsHover())
			{
				if (player->GetQtyFireRate() > 0)
				{
					soundTypes(24);
					Firerate->SetActive(true);
					player->SetQtyFireRate(player->GetQtyFireRate() - 1);
				}
				else
				{
					soundTypes(15);
				}
			}
			else if (Power_Damage->GetIsHover())
			{
				if (player->GetQtyDamage() > 0)
				{
					soundTypes(24);
					Damage->SetActive(true);
					player->SetQtyDamage(player->GetQtyDamage() - 1);
				}
				else
				{
					soundTypes(15);
				}
			}
			else if (Power_BackupTank->GetIsHover())
			{
				if (player->GetQtyTank() > 0)
				{
					if (Backup_Tank->GetReady())
					{
						soundTypes(22);
						Backup_Tank->SetActive(true);
						player->SetQtyTank(player->GetQtyTank() - 1);
					}
				}
				else
				{
					soundTypes(15);
				}
			}
			else if (Unit_Infantry->GetIsHover())
			{
				soundTypes(16);
				selection = 1;
			}
			else if (Unit_Tank->GetIsHover())
			{
				soundTypes(16);
				selection = 2;
			}
			else if (Unit_Heavy->GetIsHover())
			{
				soundTypes(16);
				selection = 3;
			}
			else if (Unit_Sniper->GetIsHover())
			{
				soundTypes(16);
				selection = 4;
			}
			else if (Unit_Mine->GetIsHover())
			{
				soundTypes(16);
				selection = 5;
			}
			else if (Unit_Barricade->GetIsHover())
			{
				soundTypes(16);
				selection = 6;
			}
		}
	}
	
	if (winscreen == true && progress <= 5)
	{ 
		// End-of-round bonus selection menu - Allow clicks to register

		// Placeholder Bonuses

		// Bonus 1: Alpha Damage +10%
		if (Bonus_Attack->GetIsHover())
		{
			if (player->GetBonus()-10 > 0)
			{
				soundTypes(23);
				Bonus_MultAttack *= 1.1f;
				player->SetBonus(player->GetBonus()-10);
			}
			else
			{
				soundTypes(15);
			}
		}


		// Bonus 2: Durability +10%
		if (Bonus_Armour->GetIsHover())
		{
			if (player->GetBonus()-10 > 0)
			{
				soundTypes(23);
				Bonus_MultArmour *= 1.1f;
				player->SetBonus(player->GetBonus()-10);
			}
			else
			{
				soundTypes(15);
			}
		}

		// Bonus 3: Loot Drop +10%
		if (Bonus_Dollar->GetIsHover())
		{
			if (player->GetBonus()-10 > 0)
			{
				soundTypes(23);
				Bonus_MultDollar *= 1.1f;
				player->SetBonus(player->GetBonus()-10);
			}
			else
			{
				soundTypes(15);
			}
		}



		if (Bonus_PShd->GetIsHover())
		{
			if (player->GetBonus()-5 > 0)
			{
				soundTypes(23);
				player->SetQtyShield(player->GetQtyShield()+1);
				player->SetBonus(player->GetBonus()-5);
			}
			else
			{
				soundTypes(15);
			}
		}

		if (Bonus_PHealth->GetIsHover())
		{
			if (player->GetBonus()-4 > 0)
			{
				soundTypes(23);
				player->SetQtyBaseHealth(player->GetQtyBaseHealth()+1);
				player->SetBonus(player->GetBonus()-4);
			}
			else
			{
				soundTypes(15);
			}
		}

		if (Bonus_PFirerate->GetIsHover())
		{
			if (player->GetBonus()-3 > 0)
			{
				soundTypes(23);
				player->SetQtyFireRate(player->GetQtyFireRate()+1);
				player->SetBonus(player->GetBonus()-3);
			}
			else
			{
				soundTypes(15);
			}
		}

		if (Bonus_PDamage->GetIsHover())
		{
			if (player->GetBonus()-3 > 0)
			{
				soundTypes(23);
				player->SetQtyDamage(player->GetQtyDamage()+1);
				player->SetBonus(player->GetBonus()-3);
			}
			else
			{
				soundTypes(15);
			}
		}

		if (Bonus_PTank->GetIsHover())
		{
			if (player->GetBonus()-6 > 0)
			{
				soundTypes(23);
				player->SetQtyTank(player->GetQtyTank()+1);
				player->SetBonus(player->GetBonus()-6);
			}
			else
			{
				soundTypes(15);
			}
		}

		//For Win Lose Screen
		if (WinLose_MainMenu->GetIsHover())
		{
			Save();
			cout << " Back To Main Menu!" << endl;
			winscreen = false;
			clearmap();
			CGameStateManager::getInstance()->ChangeState(CMenuState::Instance());
			progress = 1;
		}

		if (WinLose_NextLevel->GetIsHover())
		{
			countcheck = 0;
			playSound(a);
			winscreen = false;
			minigame = false;
			se->stopAllSounds();
			cout << " Loading Next Level!" << endl;
			spawntimer = 0.0f;
			player->SetHealth(100);
			player->SetGold(1000 + CMiniGame::Instance()->GetMGGold());
			//player->SetGold(1000);
			clearmap();
			progress++;
			loadlevel();
			tEnemyProgress->SetEnemyCounter(0);
			LoadSpawn();
		}


		if (WinLose_RestartLevel->GetIsHover())
		{
			winscreen = false;
			cout << " Restart Level!" << endl;
			spawntimer = 0.0f;
			player->SetHealth(player->GetMaxHealth());
			//player->SetGold(1000 + theMiniGame->GetMGGold());
			player->SetGold(1000);
			clearmap();
			loadlevel();

			tEnemyProgress->SetEnemyCounter(0);
			LoadSpawn();
		}
		//Load Mini Game only at level 3
		if (progress == 3)
		{
			if (WinLose_MiniGame->GetIsHover())
			{
				sound.stop();
				CMiniGame::Instance()->SetInGame(true);
				CGameStateManager::getInstance()->ChangeState(CMiniGame::Instance());
				cout << "Launching Mini Game!" << endl;
			}
		}
	} 

	else if (winscreen == true && progress > 5)
	{
		if (WinLose_MainMenu->GetIsHover())
		{
			Save();
			cout << " Back To Main Menu!" << endl;
			winscreen = false;
			clearmap();
			CGameStateManager::getInstance()->ChangeState(CMenuState::Instance());
			progress = 1;
		}

		if (WinLose_RestartLevel->GetIsHover())
		{
			winscreen = false;
			cout << " Restart Level!" << endl;
			//CGameStateManager::getInstance()->ChangeState(CPlayState::Instance());
			spawntimer = 0.0f;
			player->SetHealth(player->GetMaxHealth());
			player->SetGold(1000);
			clearmap();
			loadlevel();
			tEnemyProgress->SetEnemyCounter(0);
			LoadSpawn();
		}
	}


	else if (losescreen == true)
	{
		if (WinLose_MainMenu->GetIsHover())
		{
			cout << " Back To Main Menu!" << endl;
			losescreen = false;
			clearmap();
			CGameStateManager::getInstance()->ChangeState(CMenuState::Instance());
			progress = 1;
		}

		if (WinLose_RestartLevel->GetIsHover())
		{
			losescreen = false;
			cout << " Restart Level!" << endl;
			spawntimer = 0.0f;
			player->SetHealth(player->GetMaxHealth());
			player->SetGold(1000);
			clearmap();
			loadlevel();

			tEnemyProgress->SetEnemyCounter(0);
			LoadSpawn();

		}
	}
}

void CPlayState::Update(float dt)
{
	player->SetHealth(player->GetHealth() - 1);
	// Win lose conditions
	//winscreen = true;
	if (tEnemyProgress->GetEnemyCounter() <= 0)
	{
		winscreen = true;
	}

	if (player->GetHealth() <= 0)
	{
		player->SetHealth(0);
		//losescreen = true;
		winscreen = true;
	}

	// Despawn creep if bullet collides
	for (std::vector<Bullet *>::iterator it3 = bulletList.begin(); it3 != bulletList.end(); ++it3)
	{
		Bullet *bullet = *it3;
		if (bullet->GetActive())
		{
			//Bullet - Tower
			for (std::vector<Tower *>::iterator it2 = towerList.begin(); it2 != towerList.end(); ++it2)
			{
				Tower *tower = *it2;
				if (tower->GetActive() && tower->GetPos().x - bullet->GetPos().x > bullet->GetRadius().x && abs(tower->GetPos().y - bullet->GetPos().y) < bullet->GetRadius().y)
				{	
					if (bullet->type == Bullet::GO_ENEMYBULLET || bullet->type == Bullet::GO_TANKBULLET)
					{
						tower->SetHealth(tower->GetHealth() - bullet->GetDamage());
						bullet->SetActive(false);
						//break;
					}

					if (tower->GetHealth() <= 0) // kill the tower
					{
						tower->SetActive(false);
						int x = (int)((tower->GetPos().x / TILE_SIZE) - 0.5f);
						int y = (int)((tower->GetPos().y / TILE_SIZE) - 0.5f);
						theMap->GetGrid(x, y)->SetOccupied(false);
						break;
					}
				}
			}

			//Bullet - Enemy
			for (std::vector<Enemy *>::iterator it2 = enemyList.begin(); it2 != enemyList.end(); ++it2)
			{
				Enemy *creep = *it2;
				if(bullet->type != Bullet::GO_TANKBULLET && bullet->type != Bullet::GO_ENEMYBULLET && creep->GetActive())
				{
					if (creep->GetPos().x - bullet->GetPos().x < bullet->GetRadius().x && abs(creep->GetPos().y - bullet->GetPos().y) < bullet->GetRadius().y 
						|| (creep->type == Enemy::ENEMY_5 && creep->collisionBox(bullet->GetPos().x, bullet->GetPos().y, creep->GetPos().x, creep->GetPos().y)))
					{
						if (bullet->GetHealth() > 0)
						{
							bullet->SetHealth(bullet->GetHealth() - 1);
							bullet->SetPos(bullet->GetPos() + 20);
						}
						else if (bullet->GetHealth() <= 0)
						{
							bullet->SetActive(false);
						}
						if (bullet->type == Bullet::GO_SLOWBULLET)
						{
							creep->SetBuff(2);
							if (creep->GetBuff() > 0)
							{
								creep->SetVel(Vector3(-7.5f, 0, 0));
								creep->SetBuff(creep->GetBuff() - dt);
							}
							else if (creep->GetBuff() <= 0)
							{
								creep->SetVel(Vector3(-15, 0, 0));
							}
						}

						creep->SetHealth(creep->GetHealth() - (bullet->GetDamage())*Bonus_MultAttack); // Damage the creep

						if (creep->GetHealth() <= 0) // kill the creep
						{
							Deathsounds();
							//soundTypes(creep->type, true);
							creep->SetActive(false);

							switch (creep->type)
							{
							case Enemy::ENEMY_1:
								player->SetGold(player->GetGold() + (Enemy::NME_Y1)*Bonus_MultDollar);
								break;

							case Enemy::ENEMY_2:
								player->SetGold(player->GetGold() + (Enemy::NME_Y2)*Bonus_MultDollar);
								break;

							case Enemy::ENEMY_3:
								player->SetGold(player->GetGold() + (Enemy::NME_Y3)*Bonus_MultDollar);
								player->SetBonus(player->GetBonus() + 1);
								break;
							}

							tEnemyProgress->SetEnemyCounter(tEnemyProgress->GetEnemyCounter() - 1);
							break;
						}
					}
				}
			}
		}
	}

	// Check if creep in range
	for (std::vector<Enemy *>::iterator it2 = enemyList.begin(); it2 != enemyList.end(); ++it2)
	{
		Enemy *creep = *it2;
		if (creep->GetActive() == true)
		{
			creep->SetFire(false);
			for (std::vector<Tower *>::iterator it = towerList.begin(); it != towerList.end(); ++it)
			{
				Tower *tower = *it;
				Vector3 bulletPos;

				if (tower->GetActive() == true && tower->type != Tower::TOWER_TYPE::TOWER_MINE)
				{
					if(creep->type == Enemy::ENEMY_5)
					{
						if(tower->GetPos().y <= creep->GetPos().y + TILE_SIZE && tower->GetPos().y >= creep->GetPos().y - TILE_SIZE && tower->GetPos().x - creep->GetPos().x > -creep->GetRange() && creep->GetPos().x > tower->GetPos().x)
							creep->SetFire(true);
					}
					else
					{
						if (tower->GetPos().y > creep->GetTopLeft().y && tower->GetPos().y < creep->GetBottomRight().y  && tower->GetPos().x - creep->GetPos().x > -creep->GetRange() && creep->GetPos().x > tower->GetPos().x)
							creep->SetFire(true);
					}

					if (creep->state == Enemy::ENEMY_ATTACK)
					{
						if ((creep->type == Enemy::ENEMY_1 || creep->type == Enemy::ENEMY_3))
						{
							soundTypes(11);
							if (tower->GetPos().y > creep->GetTopLeft().y && tower->GetPos().y < creep->GetBottomRight().y && tower->GetPos().x - creep->GetPos().x > -creep->GetRange() && creep->GetPos().x > tower->GetPos().x)
							{
								//creep->SetFire(true);
								tower->SetHealth(tower->GetHealth() - (creep->GetDamage()) / Bonus_MultArmour);
								creep->SetFireCounter(creep->GetFireRate());
								if (tower->GetHealth() <= 0)
								{
									//se->play2D("bin/sounds/towerDeath.mp3", false);
									//se->setSoundVolume(0.25);
									Deathsounds();
									int x = (int)((tower->GetPos().x / TILE_SIZE) - 0.5f);
									int y = (int)((tower->GetPos().y / TILE_SIZE) - 0.5f);
									theMap->GetGrid(x, y)->SetOccupied(false);
									tower->SetActive(false);
									creep->SetFire(false);
									break;
								}
							}
						}
						else if((creep->type == Enemy::ENEMY_2 || creep->type == Enemy::ENEMY_4))
						{
							soundTypes(7);
							Bullet* newbullet = new Bullet(static_cast<Bullet::BULLET_TYPE>(Bullet::GO_ENEMYBULLET));
							newbullet->SetActive(true);
							newbullet->SetDamage(creep->GetDamage());
							newbullet->SetPos(creep->GetPos());
							newbullet->SetVel(Vector3(-1,0,0));
							newbullet->SetSpeed(400);
							CPlayState::Instance()->GetBulletList().push_back(newbullet);
							creep->SetFireCounter(creep->GetFireRate());
							//cout << creep->GetFireCounter() << endl;
							break;
						}
						else if(creep->type == Enemy::ENEMY_5)
						{
							cout << creep->pattern << endl;
							if(creep->pattern == 0) // vertical 3
							{
								for(int i = -1; i < 2; i++)
								{
									bulletPos.Set(creep->GetPos().x, (creep->GetPos().y + i*TILE_SIZE), 0);

									Bullet* newbullet = new Bullet(static_cast<Bullet::BULLET_TYPE>(Bullet::GO_TANKBULLET));
									newbullet->SetActive(true);
									newbullet->SetDamage(creep->GetDamage());
									newbullet->SetPos(bulletPos);
									newbullet->SetVel(Vector3(-1,0,0));
									newbullet->SetSpeed(400);
									CPlayState::Instance()->GetBulletList().push_back(newbullet);
								}
								creep->pattern = 1;
							}
							else if (creep->pattern == 1)
							{
								for(int i = 0; i < 3; i++)
								{
									bulletPos.Set((creep->GetPos().x + i*TILE_SIZE), creep->GetPos().y, 0);

									Bullet* newbullet = new Bullet(static_cast<Bullet::BULLET_TYPE>(Bullet::GO_TANKBULLET));
									newbullet->SetActive(true);
									newbullet->SetDamage(creep->GetDamage());
									newbullet->SetPos(bulletPos);
									newbullet->SetVel(Vector3(-1,0,0));
									newbullet->SetSpeed(400);
									CPlayState::Instance()->GetBulletList().push_back(newbullet);
								}
								creep->pattern = 0;
							}
							creep->SetFireCounter(creep->GetFireRate());
							break;
						}
						//creep->SetFireCounter(creep->GetFireRate());
						//creep->pattern += 1;
					}

					else if(creep->state == Enemy::ENEMY_RELOADING)
					{
						creep->SetFireCounter(creep->GetFireCounter() - dt);
						//cout << creep->GetFireCounter() << endl;
						break;
					}
				}
			}
		}
	}
}

Enemy* CPlayState::FetchEnemy()
{
	Enemy *tempenemy = NULL;
	if (tempenemy != NULL)
	{
		delete tempenemy;
		tempenemy = NULL;
		free(tempenemy);
	}
	for (std::vector<Enemy *>::iterator it = enemyList.begin(); it != enemyList.end(); ++it)
	{
		//Enemy *enemy = *it;
		tempenemy = *it;
		if (!tempenemy->GetActive())
		{
			tempenemy->SetActive(true);
			return tempenemy;
		}
	}

	tempenemy = new Enemy();
	//tempenemy->SetActive(true);
	//enemyList.push_back(tempenemy);
	
	return tempenemy;
}

Tower *CPlayState::FetchTower()
{
	Tower *temptower = NULL;
	if (temptower != NULL)
	{
		delete temptower;
		temptower = NULL;
		free(temptower);
	}
	for (std::vector<Tower *>::iterator it = towerList.begin(); it != towerList.end(); ++it)
	{
		temptower = *it;
		if (!temptower->GetActive())
		{
			temptower->SetActive(true);
			return temptower;
		}
	}
	temptower = new Tower(static_cast<Tower::TOWER_TYPE>(selection));
	//temptower->SetActive(true);
	//towerList.push_back(temptower);
	return temptower;
}

Spawn* CPlayState::FetchSpawn()
{
	for (std::vector<Spawn *>::iterator it = spawnList.begin(); it != spawnList.end(); ++it)
	{
		Spawn *tempspawn = *it;
		return tempspawn;
	}
	Spawn *tempspawn = new Spawn();
	spawnList.push_back(tempspawn);
	return tempspawn;
}

void CPlayState::RenderStringOnScreen(float x, float y, const char* quote)
{
	int length = strlen(quote);
	glRasterPos2f(x, y);

	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, quote[i]);
	}
}

void CPlayState::RenderBackground(void)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	// Draw Background image
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, BackgroundTexture[0].texID);
	glBegin(GL_QUADS);
	//int height = SCREEN_HEIGHT * 1.333 / 1.5;
	glTexCoord2f(0, 0); glVertex2f(0, SCREEN_HEIGHT);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void CPlayState::Load()
{
	ifstream inData;
	int length;

	string type;
	string value;

	inData.open("save/player.txt");
	while (!inData.eof()) {
		getline(inData, type, ',');
		getline(inData, value, '\n');
		{
			if (type == "health")
			{
				player->SetHealth(stoi(value));
			}
			else if (type == "shield")
			{
				player->SetShield(stoi(value));
			}
			else if (type == "gold")
			{
				player->SetGold(stoi(value));
			}
			else if (type == "shop")
			{
				player->SetBonus(stoi(value));
			}
			else if (type == "time")
			{
				spawntimer = stof(value);
			}
			else if (type == "progress")
			{
				progress = stoi(value);
			}
			else if (type == "enemyleft")
			{
				tEnemyProgress->SetEnemyCounter(stoi(value));
			}
			else if (type == "maxenemyleft")
			{
				tEnemyProgress->SetMaxEnemyCounter(stoi(value));
			}
			else if (type == "powerhealth")
			{
				player->SetQtyBaseHealth(stoi(value));
			}
			else if (type == "powershield")
			{
				player->SetQtyShield(stoi(value));
			}
			else if (type == "powerfirerate")
			{
				player->SetQtyFireRate(stoi(value));
			}
			else if (type == "powerdamage")
			{
				player->SetQtyDamage(stoi(value));
			}
			else if (type == "powertank")
			{
				player->SetQtyTank(stoi(value));
			}
		}
	}
	inData.close();

	inData.open("save/progress.txt");
	loadlevel();
	while (!inData.eof()) {
		getline(inData, type, ',');
		{
			if (type == "tower")
			{
				//Tower *tower = new Tower();
				getline(inData, value, ',');
				Tower *tower = new Tower(static_cast<Tower::TOWER_TYPE>(stoi(value)));
				getline(inData, value, ',');
				tower->state = static_cast<Tower::State>(stoi(value));
				getline(inData, value, ',');
				tower->SetActive(stoi(value));
				getline(inData, value, ',');
				tower->SetRange(stoi(value));
				getline(inData, value, ',');
				tower->SetDamage(stoi(value));
				getline(inData, value, ',');
				tower->SetVel(Vector3(stoi(value), 0, 0));
				getline(inData, value, ',');
				tower->SetVel(Vector3(tower->GetVel().x, stoi(value), 0));
				getline(inData, value, ',');
				tower->SetVel(Vector3(tower->GetVel().x, tower->GetVel().y, stoi(value)));
				getline(inData, value, ',');
				tower->SetPos(Vector3(stoi(value), 0, 0));
				getline(inData, value, ',');
				tower->SetPos(Vector3(tower->GetPos().x, stoi(value), 0));
				getline(inData, value, ',');
				tower->SetPos(Vector3(tower->GetPos().x, tower->GetPos().y, stoi(value)));
				getline(inData, value, ',');
				tower->SetHealth(stof(value));
				getline(inData, value, ',');
				tower->SetMaxHealth(stof(value));
				getline(inData, value, ',');
				tower->SetFireRate(stof(value));
				getline(inData, value, ',');
				tower->SetFireCounter(stof(value));
				getline(inData, value, '\n');
				tower->SetFire(stoi(value));
				towerList.push_back(tower);
				int x = (int)((tower->GetPos().x * theNumOfTiles_Width / w));
				int y = (int)((tower->GetPos().y * theNumOfTiles_Height / h));
				theMap->GetGrid(x, y)->SetOccupied(true);
			}
			else if (type == "enemy")
			{
				Enemy *enemy = new Enemy();
				getline(inData, value, ',');
				enemy->type = static_cast<Enemy::ENEMY_TYPE>(stoi(value));
				getline(inData, value, ',');
				enemy->state = static_cast<Enemy::ENEMY_STATE>(stoi(value));
				getline(inData, value, ',');
				enemy->SetActive(stoi(value));
				getline(inData, value, ',');
				enemy->SetRange(stoi(value));
				getline(inData, value, ',');
				enemy->SetDamage(stoi(value));
				getline(inData, value, ',');
				enemy->SetVel(Vector3(stoi(value), 0, 0));
				getline(inData, value, ',');
				enemy->SetVel(Vector3(enemy->GetVel().x, stoi(value), 0));
				getline(inData, value, ',');
				enemy->SetVel(Vector3(enemy->GetVel().x, enemy->GetVel().y, stoi(value)));
				getline(inData, value, ',');
				enemy->SetPos(Vector3(stoi(value), 0, 0));
				getline(inData, value, ',');
				enemy->SetPos(Vector3(enemy->GetPos().x, stoi(value), 0));
				getline(inData, value, ',');
				enemy->SetPos(Vector3(enemy->GetPos().x, enemy->GetPos().y, stoi(value)));
				getline(inData, value, ',');
				enemy->SetHealth(stof(value));
				getline(inData, value, ',');
				enemy->SetMaxHealth(stof(value));
				getline(inData, value, ',');
				enemy->SetFireRate(stof(value));
				getline(inData, value, ',');
				enemy->SetFireCounter(stof(value));
				getline(inData, value, '\n');
				enemy->SetFire(stoi(value));
				enemyList.push_back(enemy);
			}
			else if (type == "bullet")
			{
				Bullet *bullet = new Bullet();
				getline(inData, value, ',');
				bullet->type = static_cast<Bullet::BULLET_TYPE>(stoi(value));
				getline(inData, value, ',');
				bullet->SetActive(stoi(value));
				getline(inData, value, ',');
				bullet->SetVel(Vector3(stoi(value), 0, 0));
				getline(inData, value, ',');
				bullet->SetVel(Vector3(bullet->GetVel().x, stoi(value), 0));
				getline(inData, value, ',');
				bullet->SetVel(Vector3(bullet->GetVel().x, bullet->GetVel().y, stoi(value)));
				getline(inData, value, ',');
				bullet->SetPos(Vector3(stoi(value), 0, 0));
				getline(inData, value, ',');
				bullet->SetPos(Vector3(bullet->GetPos().x, stoi(value), 0));
				getline(inData, value, ',');
				bullet->SetPos(Vector3(bullet->GetPos().x, bullet->GetPos().y, stoi(value)));
				getline(inData, value, ',');
				bullet->SetRadius(Vector3(stoi(value), 0, 0));
				getline(inData, value, ',');
				bullet->SetRadius(Vector3(bullet->GetRadius().x, stoi(value), 0));
				getline(inData, value, ',');
				bullet->SetRadius(Vector3(bullet->GetRadius().x, bullet->GetRadius().y, stoi(value)));
				getline(inData, value, ',');
				bullet->SetHealth(stoi(value));
				getline(inData, value, '\n');
				bullet->SetDamage(stoi(value));
				bulletList.push_back(bullet);
			}
			else if (type == "spawnlist")
			{
				Spawn *spawn = new Spawn();
				getline(inData, value, ',');
				spawn->SetType(stoi(value));
				getline(inData, value, '\n');
				spawn->SetTime(stof(value));
				spawnList.push_back(spawn);
			}
			else if (type == "power_health")
			{
				getline(inData, value, ',');
				BaseHealth->type = static_cast<Powerup::POWER_TYPE>(stoi(value));
				getline(inData, value, ',');
				BaseHealth->SetActive(stoi(value));
				getline(inData, value, ',');
				BaseHealth->SetReady(stoi(value));
				getline(inData, value, ',');
				BaseHealth->SetValue(stof(value));
				getline(inData, value, ',');
				BaseHealth->SetDuration(stof(value));
			}
			else if (type == "power_shield")
			{
				getline(inData, value, ',');
				Shield->type = static_cast<Powerup::POWER_TYPE>(stoi(value));
				getline(inData, value, ',');
				Shield->SetActive(stoi(value));
				getline(inData, value, ',');
				Shield->SetReady(stoi(value));
				getline(inData, value, ',');
				Shield->SetValue(stof(value));
				getline(inData, value, ',');
				Shield->SetDuration(stof(value));
			}
			else if (type == "power_firerate")
			{
				getline(inData, value, ',');
				Firerate->type = static_cast<Powerup::POWER_TYPE>(stoi(value));
				getline(inData, value, ',');
				Firerate->SetActive(stoi(value));
				getline(inData, value, ',');
				Firerate->SetReady(stoi(value));
				getline(inData, value, ',');
				Firerate->SetValue(stof(value));
				getline(inData, value, ',');
				Firerate->SetDuration(stof(value));
			}
			else if (type == "power_damage")
			{
				getline(inData, value, ',');
				Damage->type = static_cast<Powerup::POWER_TYPE>(stoi(value));
				getline(inData, value, ',');
				Damage->SetActive(stoi(value));
				getline(inData, value, ',');
				Damage->SetReady(stoi(value));
				getline(inData, value, ',');
				Damage->SetValue(stof(value));
				getline(inData, value, ',');
				Damage->SetDuration(stof(value));
			}
			else if (type == "power_tank")
			{
				getline(inData, value, ',');
				Backup_Tank->type = static_cast<Powerup::POWER_TYPE>(stoi(value));
				getline(inData, value, ',');
				Backup_Tank->SetActive(stoi(value));
				getline(inData, value, ',');
				Backup_Tank->SetReady(stoi(value));
				getline(inData, value, ',');
				Backup_Tank->SetValue(stof(value));
				getline(inData, value, ',');
				Backup_Tank->SetDuration(stof(value));
			}
			else if (type == "tank")
			{
				getline(inData, value, ',');
				backupTank->SetActive(stoi(value));
				getline(inData, value, ',');
				backupTank->state = static_cast<Tank::State>(stoi(value));
				getline(inData, value, ',');
				backupTank->SetInPosition(stoi(value));
				getline(inData, value, ',');
				backupTank->SetPos(Vector3(stoi(value), 0, 0));
				getline(inData, value, ',');
				backupTank->SetPos(Vector3(backupTank->GetPos().x, stoi(value), 0));
			}
		}
	}
	inData.seekg(0, ios::end); // put the cursor to the end of the file
	length = inData.tellg();
	if (length == -1) // check if the file is empty
	{
		progress = 1;
		LoadSpawn();
		loadlevel();
	}
	inData.close();
}

void CPlayState::Save()
{
	ofstream file("save/player.txt");
	if (file.is_open())
	{
		file << "health, " << player->GetHealth() << "\n";
		file << "shield, " << player->GetShield() << "\n";
		file << "gold, " << player->GetGold() << "\n";
		file << "shop, " << player->GetBonus() << "\n";
		file << "time, " << spawntimer << "\n";
		file << "progress, " << progress << "\n";
		file << "enemyleft, " << tEnemyProgress->GetEnemyCounter() << "\n";
		file << "maxenemyleft, " << tEnemyProgress->GetMaxEnemyCounter() << "\n";
		file << "powerhealth, " << player->GetQtyBaseHealth() << "\n";
		file << "powershield, " << player->GetQtyShield() << "\n";
		file << "powerfirerate, " << player->GetQtyFireRate() << "\n";
		file << "powerdamage, " << player->GetQtyDamage() << "\n";
		file << "powertank, " << player->GetQtyTank() << "\n";
		file.close();
	}
	else
	{
		cout << "Unable to save stats!" << endl;
	}

	ofstream file2("save/progress.txt");
	if (file2.is_open())
	{
		for (unsigned int i = 0; i < towerList.size(); ++i)
		{
			if (towerList[i]->GetActive())
			{
				if (towerList[i]->GetPos().x != 0)
				{
					file2 << "tower, " << towerList[i]->type << ", " << towerList[i]->state << ", " << towerList[i]->GetActive() << ", " << towerList[i]->GetRange() << ", " << towerList[i]->GetDamage() << ", "
						<< towerList[i]->GetVel().x << ", " << towerList[i]->GetVel().y << ", " << towerList[i]->GetVel().z << ", "
						<< towerList[i]->GetPos().x << ", " << towerList[i]->GetPos().y << ", " << towerList[i]->GetPos().z << ", "
						<< towerList[i]->GetHealth() << ", " << towerList[i]->GetMaxHealth() << ", " << towerList[i]->GetFireRate() << ", " << towerList[i]->GetFireCounter() << ", " << towerList[i]->GetFire() << "\n";
				}
			}
		}
		for (unsigned int j = 0; j < enemyList.size(); ++j)
		{
			if (enemyList[j]->GetActive())
			{
				if (enemyList[j]->GetPos().x != 0)
				{
					file2 << "enemy, " << enemyList[j]->type << ", " << enemyList[j]->state << ", " << enemyList[j]->GetActive() << ", " << enemyList[j]->GetRange() << ", " << enemyList[j]->GetDamage() << ", "
						<< enemyList[j]->GetVel().x << ", " << enemyList[j]->GetVel().y << ", " << enemyList[j]->GetVel().z << ", "
						<< enemyList[j]->GetPos().x << ", " << enemyList[j]->GetPos().y << ", " << enemyList[j]->GetPos().z << ", "
						<< enemyList[j]->GetHealth() << ", " << enemyList[j]->GetMaxHealth() << ", " << enemyList[j]->GetFireRate() << ", " << enemyList[j]->GetFireCounter() << ", " << enemyList[j]->GetFire() << "\n";
				}
			}
		}
		for (unsigned int k = 0; k < bulletList.size(); ++k)
		{
			if (bulletList[k]->GetActive())
			{
				if (bulletList[k]->GetPos().x != 0)
				{
					file2 << "bullet, " << bulletList[k]->type << ", " << bulletList[k]->GetActive() << ", "
						<< bulletList[k]->GetVel().x << ", " << bulletList[k]->GetVel().y << ", " << bulletList[k]->GetVel().z << ", "
						<< bulletList[k]->GetPos().x << ", " << bulletList[k]->GetPos().y << ", " << bulletList[k]->GetPos().z << ", "
						<< bulletList[k]->GetRadius().x << ", " << bulletList[k]->GetRadius().y << ", " << bulletList[k]->GetRadius().z << ", "
						<< bulletList[k]->GetHealth() << ", " << bulletList[k]->GetDamage() << "\n";
				}
			}
		}

		//Spawn *spawn = FetchSpawn();
		for (unsigned int x = 0; x < spawnList.size(); ++x)
		{
			if (spawnList[x]->GetTime() >= spawntimer)
			{
				file2 << "spawnlist, " << spawnList[x]->GetType() << ", " << spawnList[x]->GetTime() << "\n";
			}
		}
		/*delete spawn;
		spawn = NULL;*/

		// For power ups
		file2 << "power_health, " << BaseHealth->type << ", " << BaseHealth->GetActive() << ", " << BaseHealth->GetReady() << ", " << BaseHealth->GetValue() << ", "
			<< BaseHealth->GetDuration() << "\n";
		file2 << "power_shield, " << Shield->type << ", " << Shield->GetActive() << ", " << Shield->GetReady() << ", " << Shield->GetValue() << ", "
			<< Shield->GetDuration() << "\n";
		file2 << "power_firerate, " << Firerate->type << ", " << Firerate->GetActive() << ", " << Firerate->GetReady() << ", " << Firerate->GetValue() << ", "
			<< Firerate->GetDuration() << "\n";
		file2 << "power_damage, " << Damage->type << ", " << Damage->GetActive() << ", " << Damage->GetReady() << ", " << Damage->GetValue() << ", "
			<< Damage->GetDuration() << "\n";
		file2 << "power_tank, " << Backup_Tank->type << ", " << Backup_Tank->GetActive() << ", " << Backup_Tank->GetReady() << ", " << Backup_Tank->GetValue() << ", "
			<< Backup_Tank->GetDuration() << "\n";

		file2 << "tank, " << backupTank->GetActive() << ", "  << backupTank->state << ", " << backupTank->GetInPosition() << ", " 
			<< backupTank->GetPos().x << ", " << backupTank->GetPos().y << "\n";

		file2.close();
	}
	else
	{
		cout << "Unable to save progress!" << endl;
	}
}

void CPlayState::UpdateSpawn()
{
	for (std::vector<Spawn *>::iterator it = spawnList.begin(); it != spawnList.end();)
	{
		Spawn *spawn = *it;
		Enemy *creep = NULL;
		if (creep != NULL)
		{
			delete creep;
			creep = NULL;
		}
		if (spawn->GetTime() <= spawntimer)
		{
			//if(creep = FetchEnemy())
			{
				creep = new Enemy(static_cast<Enemy::ENEMY_TYPE>(spawn->GetType()));
				creep->SetActive(true);
				//creep->type = static_cast<Enemy::ENEMY_TYPE>(spawn->GetType());
				creep->SetAtt(enemyClone[creep->type - 1]->GetFireRate(), enemyClone[creep->type - 1]->GetDamage(),
					enemyClone[creep->type - 1]->GetRange(), enemyClone[creep->type - 1]->GetHealth(), enemyClone[creep->type - 1]->GetSpeed());
				creep->SetMaxHealth(enemyClone[creep->type - 1]->GetHealth());
				creep->SetVel(Vector3(-15 * creep->GetSpeed(), 0, 0));
				if(creep->type == Enemy::ENEMY_5)
					creep->SetPos(Vector3(SCREEN_WIDTH, (int)(TILE_SIZE* 3.5f), 0));
				else
					creep->SetPos(Vector3(SCREEN_WIDTH, (int)(((rand() % 5 + 1) + 0.5f) * TILE_SIZE), 0));
				enemyList.push_back(creep);
			}
			delete spawn;
			it = spawnList.erase(it);
			spawn = NULL;
			free(spawn);
		}
		else
		{
			++it;
			delete creep;
			creep = NULL;
			free(creep);
		}
	}
}

void CPlayState::LoadSpawn()
{
	ifstream inData;

	string index;
	string time;

	if (progress == 1)
	{
		inData.open("save/spawn.txt");
	}
	else if (progress == 2)
	{
		inData.open("save/spawn2.txt");
	}
	else if (progress == 3)
	{
		inData.open("save/spawn3.txt");
	}
	else if (progress == 4)
	{
		inData.open("save/spawn4.txt");
	}
	else if (progress == 5)
	{
		inData.open("save/spawn5.txt");
	}
	else if (progress == 6)
	{
		inData.open("save/spawn6.txt");
	}

	while (!inData.eof()) {
		Spawn *spawn = new Spawn();

		getline(inData, index, ',');
		getline(inData, time, '\n');

		spawn->SetType(stoi(index));
		spawn->SetTime(stof(time));
		tEnemyProgress->SetEnemyCounter(tEnemyProgress->GetEnemyCounter() + 1);
		tEnemyProgress->SetMaxEnemyCounter(tEnemyProgress->GetEnemyCounter());
		spawnList.push_back(spawn);
	}
	inData.close();
}

void CPlayState::LoadAtt()
{
	ifstream inData;

	string index;
	string value;

	inData.open("save/tower.txt");
	while (!inData.eof())
	{
		//Tower *tower = new Tower();

		getline(inData, index, ',');
		Tower *tower = new Tower(static_cast<Tower::TOWER_TYPE>(stoi(index)));
		getline(inData, value, ',');
		tower->SetFireRate(stof(value));
		getline(inData, value, ',');
		tower->SetCost(stoi(value));
		getline(inData, value, ',');
		tower->SetDamage(stoi(value));
		getline(inData, value, ',');
		tower->SetRange(stoi(value));
		getline(inData, value, '\n');
		tower->SetHealth(stoi(value));
		tower->SetMaxHealth(tower->GetHealth());
		towerClone.push_back(tower);
	}
	inData.close();

	inData.open("save/creep.txt");
	while (!inData.eof())
	{
		Enemy *creep = new Enemy();

		getline(inData, index, ',');
		creep->type = static_cast<Enemy::ENEMY_TYPE>(stoi(index));
		getline(inData, value, ',');
		creep->SetFireRate(stof(value));
		getline(inData, value, ',');
		creep->SetDamage(stoi(value));
		getline(inData, value, ',');
		creep->SetRange(stoi(value));
		getline(inData, value, ',');
		creep->SetHealth(stoi(value));
		creep->SetMaxHealth(creep->GetHealth());
		getline(inData, value, '\n');
		creep->SetSpeed(stof(value));
		enemyClone.push_back(creep);
	}
	inData.close();
}

void CPlayState::playSound(int a)
{
	if (audioplay == true)
	{
		if (soundon == true)
		{
			if (a == 1)
			{
				sound.stop();
				sound.setFileName("bin/sounds/BGM.mp3");
				//sound.setVolume(25);
				sound.playSoundThreaded();
			}
			else if (a == 2)
			{
				sound.stop();
				sound.setFileName("bin/sounds/tempBGM.mp3");
				sound.playSoundThreaded();
			}
			else if (a == 3)
			{
				sound.stop();
				sound.setFileName("bin/sounds/Snake_Eater.mp3");
				sound.playSoundThreaded();
			}
			else if (a == 4)
			{
				sound.stop();
				sound.setFileName("bin/sounds/Victory_bgm.mp3");
				sound.playSoundThreaded();
			}
		}
	}
}

void CPlayState::shooting(bool firing)
{
	se->play2D("bin/sounds/smb2_pickup.wav", false);
	se->setSoundVolume(0.3);
	firing = false;
}

void CPlayState::soundTypes(int type)
{
		if (audioplay == true)
		{
			int random = RNGesus();
			switch (type)
			{
				case 1:
					se->play2D("bin/sounds/chipDeath.mp3", false);
					break;
				case 2:
					se->play2D("bin/sounds/cardDeath.mp3", false);
					break;
				case 3:
					se->play2D("bin/sounds/humanDeath.mp3", false);
					break;
				case 4:
					se->play2D("bin/sounds/xplosionSFX.wav", false);
					break;
				case 5:
					se->play2D("bin/sounds/laserSFX.mp3", false);
					break;
				case 6:
					se->play2D("bin/sounds/sniper.wav", false);
					break;
				case 7:
					se->play2D("bin/sounds/Soldier.wav", false);
					break;
				case 8:
					se->play2D("bin/sounds/shells.wav", false);
					break;
				case 9:
					se->play2D("bin/sounds/Missile.wav", false);
					break;
				case 10:
					se->play2D("bin/sounds/TankFire.wav", false);
					break;
				case 11:
					switch (random)
					{
					case 1:
						se->play2D("bin/sounds/Hit_1.wav", false);
						se->setSoundVolume(0.25);
						break;
					case 2:
						se->play2D("bin/sounds/Hit_2.wav", false);
						se->setSoundVolume(0.25);
						break;
					case 3:
						se->play2D("bin/sounds/Hit_3.wav", false);
						se->setSoundVolume(0.25);
						break;
					}
					break;
			case 12:
				se->play2D("bin/sounds/mission_start.wav", false);
				break;
			case 13:
				se->play2D("bin/sounds/mission_complete.mp3", false);
				break;
			case 14:
				if (winscreen != true)
				se->play2D("bin/sounds/unit.mp3", false);
				break;
			case 15:
				se->play2D("bin/sounds/no_money.wav", false);
				break;
			case 16:
				se->play2D("bin/sounds/sliderMove.wav", false);
				break;
			case 17:
				if (winscreen != true)
				se->play2D("bin/sounds/unit_missile.mp3", false);
				break;
			case 18:
				if (winscreen != true)
				se->play2D("bin/sounds/unit_sniper.wav", false);
				break;
			case 19:
				if (winscreen != true)
				se->play2D("bin/sounds/unit_barricade.wav", false);
				break;
			case 20:
				se->play2D("bin/sounds/placement.wav", false);
				break;
			case 21:
				if (winscreen != true)
				se->play2D("bin/sounds/unit_mine.wav", false);
				break;
			case 22:
				if (winscreen != true)
				se->play2D("bin/sounds/unit_tank.mp3", false);
				break;
			case 23:
				se->play2D("bin/sounds/purchase.wav", false);
				break;
			case 24:
				se->play2D("bin/sounds/power_use.mp3", false);
				break;
			}
		}
}

void CPlayState::loadlevel()
{
	if (progress == 1)
	{
		theMap->LoadMap("bin/maps/MapDesign.csv", 0, 0, TILE_SIZE, TILE_SIZE);
	}

	else if (progress == 2)
	{
		theMap->LoadMap("bin/maps/MapDesign2.csv", 0, 0, TILE_SIZE, TILE_SIZE);
	}

	else if (progress == 3)
	{
		theMap->LoadMap("bin/maps/MapDesign3.csv", 0, 0, TILE_SIZE, TILE_SIZE);
	}

	else if (progress == 4)
	{
		theMap->LoadMap("bin/maps/MapDesign4.csv", 0, 0, TILE_SIZE, TILE_SIZE);
	}

	else if (progress == 5)
	{
		theMap->LoadMap("bin/maps/MapDesign5.csv", 0, 0, TILE_SIZE, TILE_SIZE);
	}

	else if (progress == 6)
	{
		theMap->LoadMap("bin/maps/MapDesign6.csv", 0, 0, TILE_SIZE, TILE_SIZE);
	}
}

void CPlayState::clearmap()
{
	/*
	for (std::vector <Tower*>::iterator it = towerList.begin(); it < towerList.end(); it++)
	{
		Tower *tower = *it;
		if (tower->GetActive())
		{
			tower->SetActive(false);
		}
	}
	for (std::vector <Enemy*>::iterator it = enemyList.begin(); it < enemyList.end(); it++)
	{
		Enemy *creep = *it;
		if (creep->GetActive())
		{
			creep->SetActive(false);
		}
	}
	for (std::vector <Bullet*>::iterator it = bulletList.begin(); it < bulletList.end(); it++)
	{
		Bullet *bullet = *it;
		if (bullet->GetActive())
		{
			bullet->SetActive(false);
		}
	} */


	/*while (bulletList.size() > 0)
	{
		Bullet *bullet = bulletList.back();
		delete bullet;
		bullet = NULL;
		bulletList.pop_back();
		free(bullet);
	}

	while (enemyList.size() > 0)
	{
		Enemy *creep = enemyList.back();
		delete creep;
		creep = NULL;
		enemyList.pop_back();
		free(creep);
	}

	while (towerList.size() > 0)
	{
		Tower *tower = towerList.back();
		delete tower;
		tower = NULL;
		towerList.pop_back();
		free(tower);
	}

	while (spawnList.size() > 0)
	{
		Spawn *spawn = spawnList.back();
		delete spawn;
		spawn = NULL;
		spawnList.pop_back();
		free(spawn);
	}*/

	towerList.clear();
	enemyList.clear();
	bulletList.clear();
	spawnList.clear();

	/*
	if (theMap != NULL)
	{
		delete theMap;
		theMap = NULL;
		free(theMap);
	} */

}

void CPlayState::RenderInfo(int x, int y)
{
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
	glTranslatef(x, y, 1);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(0, -120);
	glVertex2f(160, -120);
	glVertex2f(160, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	char temp[256];
	if (info > 0) // Unit Info
	{
		if (info == 1)
		{
			sprintf_s(temp, "Machine Gun");
		}
		else if (info == 2)
		{
			sprintf_s(temp, "Mini Tank");
		}
		else if (info == 3)
		{
			sprintf_s(temp, "RPG");
		}
		else if (info == 4)
		{
			sprintf_s(temp, "Sniper");
		}
		else if (info == 5)
		{
			sprintf_s(temp, "Mine");
		}
		else if (info == 6)
		{
			sprintf_s(temp, "Barricade");
		}
		RenderStringOnScreen(x + 4, y - 98, temp);
		sprintf_s(temp, "Health: %d", towerClone[info - 1]->GetHealth());
		RenderStringOnScreen(x + 4, y - 80, temp);
		sprintf_s(temp, "Cost: %d", towerClone[info - 1]->GetCost());
		RenderStringOnScreen(x + 4, y - 62, temp);
		sprintf_s(temp, "Damage: %d", towerClone[info - 1]->GetDamage());
		RenderStringOnScreen(x + 4, y - 44, temp);
		sprintf_s(temp, "Firerate: %.2f", towerClone[info - 1]->GetFireRate());
		RenderStringOnScreen(x + 4, y - 26, temp);
		sprintf_s(temp, "Range: %d", towerClone[info - 1]->GetRange());
		RenderStringOnScreen(x + 4, y - 8, temp);
	}
	else if (info < 0) // Bonus Selection Info
	{
		if (info == -1)
		{
			sprintf_s(temp, "Increase bullet damage by 10%!");
			RenderStringOnScreen(x + 4, y - 98, temp);
			sprintf_s(temp, "Cost: 10 Medals");
			RenderStringOnScreen(x + 4, y - 80, temp);
		}
		else if (info == -2)
		{
			sprintf_s(temp, "Increase damage resistance by 10%!");
			RenderStringOnScreen(x + 4, y - 98, temp);
			sprintf_s(temp, "Cost: 10 Medals");
			RenderStringOnScreen(x + 4, y - 80, temp);
		}
		else if (info == -3)
		{
			sprintf_s(temp, "Increase gold drop by 10%!");
			RenderStringOnScreen(x + 4, y - 98, temp);
			sprintf_s(temp, "Cost: 10 Medals");
			RenderStringOnScreen(x + 4, y - 80, temp);
		}
	}
}

void CPlayState::RenderHUD()
{
	char temp[512];

	// Progression bar
	glPushMatrix();
		tEnemyProgress->DrawEnemyCounter(500, 48); // Enemy Progress Bar
	glPopMatrix();

	// Render buttons
	Button_Pause->Render();

	Unit_Infantry->Render();
	Unit_Tank->Render();
	Unit_Heavy->Render();
	Unit_Sniper->Render();
	Unit_Mine->Render();
	Unit_Barricade->Render();

	// Render player stats
	player->RenderHealthBar(75, 10);
	player->RenderShield(75, 45);

	// On screen texts
	glColor3f(1.0f, 1.0f, 1.0f);

	sprintf_s(temp, "Health: ");
	RenderStringOnScreen(10, 30, temp);
	sprintf_s(temp, "Shield: ");
	RenderStringOnScreen(10, 65, temp);

	sprintf_s(temp, " %d", player->GetHealth());
	RenderStringOnScreen(SCREEN_WIDTH*0.15, 30, temp);
	sprintf_s(temp, " / 100");
	RenderStringOnScreen(SCREEN_WIDTH*0.17, 30, temp);

	sprintf_s(temp, "Gold: %d", player->GetGold());
	RenderStringOnScreen(10, 90, temp);
	/*sprintf_s(temp, "FPS: %.2f", m_fps);
	RenderStringOnScreen(0, 30, temp);*/
	sprintf_s(temp, "Selection: %d", selection);
	RenderStringOnScreen(120, 90, temp);
	sprintf_s(temp, "Enemy left: %d", tEnemyProgress->GetEnemyCounter());
	RenderStringOnScreen(500, 32, temp);

	// Mouse over tower selection for info
	if (info != 0)
	{
		RenderInfo(mouseInfo.lastX, mouseInfo.lastY);
	}

	Power_Shield->Render();
	Power_BaseHealth->Render();
	Power_Firerate->Render();
	Power_Damage->Render();
	Power_BackupTank->Render();

	Shield->RenderDurationBar(Power_Shield->GetPosition().x, Power_Shield->GetPosition().y);
	BaseHealth->RenderDurationBar(Power_BaseHealth->GetPosition().x, Power_BaseHealth->GetPosition().y);
	Firerate->RenderDurationBar(Power_Firerate->GetPosition().x, Power_Firerate->GetPosition().y);
	Damage->RenderDurationBar(Power_Damage->GetPosition().x, Power_Damage->GetPosition().y);
	Backup_Tank->RenderDurationBar(Power_BackupTank->GetPosition().x, Power_BackupTank->GetPosition().y);

	glColor3f(1.0f, 1.0f, 1.0f);

	sprintf_s(temp, "x%d", player->GetQtyShield());
	RenderStringOnScreen(608, 666, temp);
	sprintf_s(temp, "x%d", player->GetQtyBaseHealth());
	RenderStringOnScreen(680, 666, temp);
	sprintf_s(temp, "x%d", player->GetQtyFireRate());
	RenderStringOnScreen(752, 666, temp);
	sprintf_s(temp, "x%d", player->GetQtyDamage());
	RenderStringOnScreen(824, 666, temp);
	sprintf_s(temp, "x%d", player->GetQtyTank());
	RenderStringOnScreen(896, 666, temp);
	if (pause)
	{
		if (!exitmenu)
		{
			RenderPauseMenu();
		}
		else
		{
			RenderExitMenu();
		}
	}

	if (winscreen == true)
	{
		if (countcheck != 1)
		{
			playvictory = true;
			countcheck = 1;
		}

		char temp[16];
		if (playvictory == true)
		{
			sound.stop();
			if (audioplay == true)
			{
				se->play2D("bin/sounds/victory_bgm.mp3");
				se->play2D("bin/sounds/mission_complete.mp3");
			}
			playvictory = false;
		}
		glColor3f(1.0f, 1.0f, 1.0f);

		RenderWinScreen();

		Shop_BG->Render();
		Bonus_Attack->Render();
		Bonus_Armour->Render();
		Bonus_Dollar->Render();

		Bonus_PShd->Render();
		Bonus_PHealth->Render();
		Bonus_PFirerate->Render();
		Bonus_PDamage->Render();
		Bonus_PTank->Render();

		glColor3f(0.0f, 0.0f, 0.0f);
		sprintf_s(temp, "Bonus: %d", player->GetBonus());
		RenderStringOnScreen(450, 200, temp);
		glColor3f(1.0f, 1.0f, 1.0f);

		sprintf_s(temp, "x%d", player->GetQtyShield());
		RenderStringOnScreen(260, 440, temp);

		sprintf_s(temp, "x%d", player->GetQtyBaseHealth());
		RenderStringOnScreen(360, 440, temp);

		sprintf_s(temp, "x%d", player->GetQtyFireRate());
		RenderStringOnScreen(460, 440, temp);

		sprintf_s(temp, "x%d", player->GetQtyDamage());
		RenderStringOnScreen(560, 440, temp);

		sprintf_s(temp, "x%d", player->GetQtyTank());
		RenderStringOnScreen(660, 440, temp);

		sprintf_s(temp, "%1.0f%%", 100*Bonus_MultAttack);
		RenderStringOnScreen(350, 275, temp);

		sprintf_s(temp, "%1.0f%%", 100*Bonus_MultArmour);
		RenderStringOnScreen(450, 275, temp);

		sprintf_s(temp, "%1.0f%%", 100*Bonus_MultDollar);
		RenderStringOnScreen(550, 275, temp);

		sprintf_s(temp, "5 pts");
		RenderStringOnScreen(250, 375, temp);

		sprintf_s(temp, "4 pts");
		RenderStringOnScreen(350, 375, temp);

		sprintf_s(temp, "3 pts");
		RenderStringOnScreen(450, 375, temp);

		sprintf_s(temp, "3 pts");
		RenderStringOnScreen(550, 375, temp);

		sprintf_s(temp, "6 pts");
		RenderStringOnScreen(645, 375, temp);

		sprintf_s(temp, "10 pts");
		RenderStringOnScreen(345, 260, temp);

		sprintf_s(temp, "10 pts");
		RenderStringOnScreen(445, 260, temp);

		sprintf_s(temp, "10 pts");
		RenderStringOnScreen(545, 260, temp);
	}

	else if (losescreen == true)
	{
		RenderLoseScreen();
	}

	if (minigame == true)
	{
		
	}

	RenderCursor();

	glColor3f(1.0f, 1.0f, 1.0f);
}

void CPlayState::RenderPauseMenu()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTranslatef(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
	glBindTexture(GL_TEXTURE_2D, PauseMenu.texID);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);	glVertex2f(-160, -224);
	glTexCoord2f(1, 1);	glVertex2f(160, -224);
	glTexCoord2f(1, 0);	glVertex2f(160, 224);
	glTexCoord2f(0, 0);	glVertex2f(-160, 224);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	PauseMenu_Resume->Render();
	PauseMenu_Restart->Render();
	PauseMenu_Exit->Render();
}

void CPlayState::RenderExitMenu()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTranslatef(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
	glBindTexture(GL_TEXTURE_2D, ExitMenu.texID);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);	glVertex2f(-160, -224);
	glTexCoord2f(1, 1);	glVertex2f(160, -224);
	glTexCoord2f(1, 0);	glVertex2f(160, 224);
	glTexCoord2f(0, 0);	glVertex2f(-160, 224);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	ExitMenu_Yes->Render();
	ExitMenu_No->Render();
}

void CPlayState::RenderWinScreen()
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, WinScreenTexture.texID);
	glTranslatef(0, 0, 0);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, SCREEN_HEIGHT);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	if (progress < 3)
	{
		WinLose_MainMenu->Render();
		WinLose_NextLevel->Render();
		WinLose_RestartLevel->Render();
	}
	else if (progress == 3)
	{
		WinLose_MainMenu->Render();
		WinLose_NextLevel->Render();
		WinLose_RestartLevel->Render();
		WinLose_MiniGame->Render();
	}

	else if (progress > 3)
	{
		WinLose_MainMenu->Render();
		WinLose_NextLevel->Render();
		WinLose_RestartLevel->Render();
	}

}

void CPlayState::RenderLoseScreen()
{
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, LoseScreenTexture.texID);
	glTranslatef(0, 0, 0);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, SCREEN_HEIGHT);
	glTexCoord2f(1, 0); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	glTexCoord2f(1, 1); glVertex2f(SCREEN_WIDTH, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	WinLose_MainMenu->Render();
	WinLose_RestartLevel->Render();
}

void CPlayState::RenderCursor()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTranslatef(mouseInfo.lastX, mouseInfo.lastY, 0);
	glBindTexture(GL_TEXTURE_2D, Cursor[selection - 1].texID);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 48);
	glTexCoord2f(1, 0); glVertex2f(48, 48);
	glTexCoord2f(1, 1); glVertex2f(48, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// to return the vector list to other classes so that they can push objects into it
std::vector<Bullet*>& CPlayState::GetBulletList(void)
{
	return bulletList;
}

// public function to initialize the level when changing to this state
void CPlayState::SetLevel(int level)
{
	this->progress = level;
}

// public function to set whether to load save file
void CPlayState::LoadFromFile(bool load)
{
	this->load = load;
}

void CPlayState::Deathsounds()
{
	int a = RNGesus();
	if (audioplay == true)
	{		
		switch (a)
		{
		case 1:
			se->play2D("bin/sounds/death_1.wav", false);
			break;
		case 2:
			se->play2D("bin/sounds/death_2.wav", false);
			break;
		case 3:
			se->play2D("bin/sounds/death_3.wav", false);
			break;
		}
	}
}
