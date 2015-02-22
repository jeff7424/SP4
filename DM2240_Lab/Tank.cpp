#include "Tank.h"

Tank::Tank(State state)
{
	SetDamage(50);
	SetRange(800);
	SetFire(false);
	SetFireRate(3.0f);
	SetPos(Vector3(0, 336, 0));
}

Tank::~Tank()
{

}

void Tank::ChangeState()
{
	switch (state)
	{
		case STATE_IDLE:
		{
			if (inPosition)
			{
				if (Target != NULL && Target->GetActive())
				{
					Vector3 CurrentTarget = Target->GetPos() - this->GetPos();
					if ((this->GetRange() * this->GetRange()) >= CurrentTarget.Dot(CurrentTarget))
					{
						state = STATE_LOADING;
					}
				}
			}
		}
		break;
		case STATE_FALLIN:
		{
			if (this->GetPos().x == 50)
			{
				inPosition = true;
				if (Target == NULL)
				{
					state = STATE_IDLE;
				}
				else
				{
					state = STATE_LOADING;
				}
			}
		}
		break;
		case STATE_LOADING:
		{
			if (this->GetFireCounter() <= 0.0f)
			{
				state = STATE_ATTACK;
				this->SetFireCounter(this->GetFireRate());
			}
		}
		break;
		case STATE_ATTACK:
		{
			if (Target != NULL && Target->GetActive())
			{
				Vector3 CurrentTarget = Target->GetPos() - this->GetPos();
				if ((this->GetRange() * this->GetRange()) >= CurrentTarget.Dot(CurrentTarget))
				{
					state = STATE_LOADING;
				}
			}
		}
		break;
		case STATE_FALLOUT:
		{
			if (this->GetPos().x > 0)
			{
				state = STATE_IDLE;
			}
		}
		break;
	}
}

void Tank::Respond(float dt)
{
	switch (state)
	{
	case STATE_IDLE:
		break;
	case STATE_FALLIN:
		{
			this->SetPos(Vector3(this->GetPos().x + 1, this->GetPos().y, this->GetPos().z));
		}
		break;
	case STATE_LOADING:
		{
			this->SetFireCounter(this->GetFireCounter() - dt);
		}
		break;
	case STATE_ATTACK:
		{
			if (Target != NULL)
			{
				Bullet* bullet = new Bullet();
				bullet->SetActive(true);
				bullet->SetDamage(this->GetDamage());
				CPlayState::Instance()->GetBulletList().push_back(bullet);
			}
		}
		break;
	case STATE_FALLOUT:
	{

	}
		break;
	}
}

void Tank::Update(float dt)
{
	ChangeState();
	Respond(dt);
}

void Tank::Render()
{

}