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
	//int progress;

public:
	CEnemyProgress();
	~CEnemyProgress();
	
	void SetPosX(int x);
	void SetPosY(int y);
	void SetMinLength(int l);
	void SetEnemyCounter(int e);

	void Active(bool);
	int GetPosX();
	int GetPosY();
	int GetMinLength();
	int GetEnemyCounter();

	void initEnemyCounter();

	bool Active();
	void DrawEnemyCounter(int x, int y);

};

