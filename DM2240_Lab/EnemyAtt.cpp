#include "EnemyAtt.h"
#include "TextureImage.h"

Enemy::Enemy(ENEMY_TYPE type)
: type(type)
, buff(0)
, state(ENEMY_LANE)
, speed(0)
, offsetX(0)
, offsetY(0)
, offsetY2(0)
, tilesTravelled(0)
, called (false)
, currentTile(10)	//init position tile (softcode)
{
	srand (time(NULL));
}

Enemy::~Enemy()
{

}

void Enemy::SetAtt(float firerate, int damage, int range, int health, float speed)
{
	SetHealth(health);
	SetDamage(damage);
	SetFireRate(firerate);
	SetRange(range);
	SetSpeed(speed);
}

void Enemy::SetMovement(std::vector<Enemy *> eList, int TILE_SIZE, float dt, int laneSwap)
{
	for (std::vector<Enemy *>::iterator it = eList.begin(); it != eList.end(); ++it)
	{
		Enemy *creep = *it;
		if (creep->GetActive() == true)
		{
			if (creep->GetFire() == false)
			{	
				int random = rand() % 2 + 1;
				/*if(creep->type == ENEMY_TYPE::ENEMY_1)
					cout << "offsetY : " << creep->offsetY << endl;
				cout << "lane" << laneSwap << endl;*/
				creep->offsetX = creep->GetPos().x;
				creep->tilesTravelled = (creep->offsetX / TILE_SIZE);
				creep->offsetY = (creep->GetPos().y - (TILE_SIZE/2)) / TILE_SIZE; //current y tile

				//Enemy movement
				if(creep->state == ENEMY_STATE::ENEMY_ADVANCE)
				{
					creep->SetPos(Vector3(creep->GetPos().x + creep->GetVel().x*dt, creep->GetPos().y, 0));
					creep->SetPos(Vector3(creep->GetPos().x, ((creep->offsetY* TILE_SIZE) + (TILE_SIZE/2)), 0));
				}

				//Enemy change lane
				if(creep->state == ENEMY_STATE::ENEMY_LANE)
				{
					if(creep->type == Enemy::ENEMY_1)	//after 2 tiles move up/ down
					{	
						if(!creep->called)
						{
							if(creep->offsetY > 1 && creep->offsetY < 5)
							{
								if(random == 1)
								creep->offsetY2 = creep->offsetY - 1;

								else
								creep->offsetY2 = creep->offsetY + 1;
							}
							else if(creep->offsetY == 5)
							{creep->offsetY2 = creep->offsetY - 1;}
							else if(creep->offsetY == 1)
							{creep->offsetY2 = creep->offsetY + 1;}

							creep->called = true;
						}

						if(creep->offsetY2 <= creep->offsetY)	//current y tile - 1 (move up)
						{
							creep->SetPos(Vector3(creep->GetPos().x, creep->GetPos().y + creep->GetVel().x*dt, 0));
						}

						else
						{
							creep->SetPos(Vector3(creep->GetPos().x, (creep->offsetY2* TILE_SIZE + (TILE_SIZE/2)), 0));	//pop to middle of tile		1.set postion values to TILE_SIZE
							creep->currentTile -= 2;
							creep->called = false;
						}
					}

					if(creep->type == Enemy::ENEMY_2)
					{
						if(laneSwap < creep->offsetY ) //move up
						creep->SetPos(Vector3(creep->GetPos().x, creep->GetPos().y + creep->GetVel().x*dt, 0));

						if(laneSwap > creep->offsetY ) //move down
						creep->SetPos(Vector3(creep->GetPos().x, creep->GetPos().y - creep->GetVel().x*dt, 0));
					}
				}

				//Change state
				switch (creep->type){

			/*	case Enemy::ENEMY_1:
					creep->state = ENEMY_STATE::ENEMY_ADVANCE;
					break;

				case Enemy::ENEMY_2:
					creep->state = ENEMY_STATE::ENEMY_ADVANCE;
					break;*/

				case Enemy::ENEMY_1:
					if(creep->currentTile == creep->tilesTravelled)
						creep->state = ENEMY_STATE::ENEMY_LANE;
					else
						creep->state = ENEMY_STATE::ENEMY_ADVANCE;
					break;

				case Enemy::ENEMY_2:
					if(creep->offsetY != laneSwap)
						creep->state = ENEMY_STATE::ENEMY_LANE;
					else 
						creep->state = ENEMY_STATE::ENEMY_ADVANCE;
					break;

				}
			}
		}
	}
}

void Enemy::SetBuff(float buff)
{
	this->buff = buff;
}

void Enemy::SetSpeed(float speed)
{
	this->speed = speed;
}

float Enemy::GetBuff()
{
	return buff;
}

float Enemy::GetSpeed()
{
	return speed;
}

void Enemy::Update(float dt)
{

}