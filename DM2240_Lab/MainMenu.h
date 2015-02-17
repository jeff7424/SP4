#pragma once
class CMainMenu
{
private:
	int state;

public:
	CMainMenu();
	~CMainMenu();

	enum states {
		STATE_START,
		STATE_EXIT
	};
};

