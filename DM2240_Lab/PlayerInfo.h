#pragma once

class PlayerInfo {
private:
	int life;
	int gold;
	int addiction;

public:
	PlayerInfo();
	~PlayerInfo();

	void SetLife(int life);
	void SetGold(int gold);
	void SetAddiction(int addiction);

	int GetLife();
	int GetGold();
	int GetAddiction();
};