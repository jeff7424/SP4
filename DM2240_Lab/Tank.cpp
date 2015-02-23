#include "Tank.h"

Tank::Tank(State state)
{
	this->state = STATE_IDLE;
	SetActive(false);
	SetDamage(10);
	SetRange(800);
	SetFire(false);
	SetFireRate(3.0f);
	SetPos(Vector3(0, 336, 0));
	Target = NULL;
	inPosition = false;
}

Tank::~Tank()
{

}

void Tank::ChangeState()
{
	if (state == STATE_IDLE)
	{
		if (this->GetActive())
		{
			state = STATE_FALLIN;
		}
		if (inPosition)
		{
			if (Target != NULL && Target->GetActive() && Target->GetHealth() > 0)
			{
				Vector3 CurrentTarget = Target->GetPos() - this->GetPos();
				if ((this->GetRange() * this->GetRange()) >= CurrentTarget.Dot(CurrentTarget))
				{
					state = STATE_LOADING;
				}
			}
		}
	}
	else if (state == STATE_FALLIN)
	{
		if (this->GetPos().x >= 50)
		{
			inPosition = true;
			//this->SetPos(Vector3(50, this->GetPos().y, this->GetPos().z));
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
	else if (state == STATE_LOADING)
	{
		if (this->GetActive() && inPosition)
		{
			if (Target != NULL && Target->GetActive() && Target->GetHealth() > 0)
			{
				Vector3 CurrentTarget = Target->GetPos() - this->GetPos();
				if ((this->GetRange() * this->GetRange()) >= CurrentTarget.Dot(CurrentTarget))
				{
					if (this->GetFireCounter() <= 0.0f)
					{
						state = STATE_ATTACK;
						this->SetFireCounter(this->GetFireRate());
					}
				}
			}
			else
				state = STATE_IDLE;
		}
		else
			state = STATE_FALLOUT;
	}
	else if (state == STATE_ATTACK)
	{
		if (this->GetActive() && inPosition)
		{
			if (Target != NULL && Target->GetActive() && Target->GetHealth() > 0)
			{
				Vector3 CurrentTarget = Target->GetPos() - this->GetPos();
				//if ((this->GetRange() * this->GetRange()) >= CurrentTarget.Dot(CurrentTarget))
				//{
				state = STATE_LOADING;
				//}
			}
			else
				state = STATE_IDLE;
		}
		else
			state = STATE_FALLOUT;
	}
	else if (state == STATE_FALLOUT)
	{
		if (this->GetPos().x <= 0)
		{
			//this->SetPos(Vector3(0, this->GetPos().y, this->GetPos().z));
			state = STATE_IDLE;
			inPosition = false;
		}
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
			if (!inPosition)
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
			if (Target != NULL && Target->GetActive() && Target->GetHealth() > 0 && inPosition)
			{
				Bullet* bullet = new Bullet();
				bullet->SetActive(true);
				bullet->SetDamage(this->GetDamage());
				bullet->SetPos(this->GetPos());
				bullet->SetVel(Target->GetPos() - this->GetPos());
				bullet->SetSpeed(400);
				CPlayState::Instance()->GetBulletList().push_back(bullet);
			}
		}
		break;
	case STATE_FALLOUT:
		{
			if (inPosition)
				this->SetPos(Vector3(this->GetPos().x - 1, this->GetPos().y, this->GetPos().z));
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
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBindTexture(GL_TEXTURE_2D, body.texID);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTranslatef(GetPos().x, GetPos().y, GetPos().z);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-96, -96);
	glTexCoord2f(1, 0); glVertex2f(96, -96);
	glTexCoord2f(1, 1); glVertex2f(96, 96);
	glTexCoord2f(0, 1); glVertex2f(-96, 96);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Tank::GetTarget(Enemy* enemy)
{
	if (Enemy *test = dynamic_cast<Enemy*>(enemy))
	{
		if (enemy == Target)
			return;
		else if (Target == NULL || !Target->GetActive())
		{
			Target = enemy;
			return;
		}
		else if (Target != NULL && Target->GetActive())
		{
			Vector3 CurrentTarget = Target->GetPos() - this->GetPos();
			Vector3 ThisTarget = enemy->GetPos() - this->GetPos();

			if (ThisTarget.Dot(ThisTarget) < CurrentTarget.Dot(CurrentTarget))
			{
				Target = enemy;
			}
		}
	}
}