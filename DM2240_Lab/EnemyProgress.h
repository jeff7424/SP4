#pragma once

#include "Vector3.h"
#include <GL/glut.h>

class CEnemyProgress
{
private:
	Vector3 pos;
	int minlength;
	bool active;
	int enemycounter;
	int maxenemycounter;

public:
	CEnemyProgress();
	~CEnemyProgress();
	
	void SetPosX(int x);
	void SetPosY(int y);
	void SetMinLength(int l);
	void SetEnemyCounter(int e);
	void SetMaxEnemyCounter(int e);

	void Active(bool);
	int GetPosX();
	int GetPosY();
	int GetMinLength();
	int GetEnemyCounter();
	int GetMaxEnemyCounter();

	void initEnemyCounter(int counter);

	bool Active();
	void DrawEnemyCounter(int x, int y);
};

