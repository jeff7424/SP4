#ifndef TANK_H_
#define TANK_H_

#pragma once

#include "PlayState.h"
#include "EnemyAtt.h"
#include "Units.h"
#include "TextureImage.h"
#include "Bullet.h"

class Tank : public Units
{
private:
	TextureImage body;
	TextureImage cannon;
	Enemy *Target;
	bool inPosition;

public:
	enum State {
		STATE_IDLE = 0,
		STATE_FALLIN,
		STATE_LOADING,
		STATE_ATTACK,
		STATE_FALLOUT
	};

	State state;

	Tank(State state = STATE_IDLE);
	~Tank();

	void ChangeState();
	void Respond(float dt);
	void Update(float dt);
	void Render();
	void GetTarget(Enemy*);
};
#endif