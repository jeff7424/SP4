#pragma once
#include "EnemyAtt.h"

class Spawn {
private:
	bool active;
	float second;
	int type;

public:
	Spawn();
	~Spawn();

	void SetType(int type);
	void SetTime(float time);
	void SetActive(bool active);

	int GetType();
	float GetTime();
	bool GetActive();
};