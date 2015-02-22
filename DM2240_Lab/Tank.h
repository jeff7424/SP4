#pragma once

#include "EnemyAtt.h"
#include "Units.h"
#include "TextureImage.h"
#include "PlayState.h"
#include "Bullet.h"

class Tank : public Units
{
private:
	static TextureImage body;
	static TextureImage cannon;
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
};