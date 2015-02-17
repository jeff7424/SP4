#include "PlayerInfo.h"
#include <fstream>
#include <string>

using namespace std;

PlayerInfo::PlayerInfo()
: life(3)
, gold(700)
, addiction(0)
{

}

PlayerInfo::~PlayerInfo()
{

}

void PlayerInfo::SetLife(int life)
{
	this->life = life;
}

void PlayerInfo::SetGold(int gold)
{
	this->gold = gold;
}

int PlayerInfo::GetLife()
{
	return life;
}

int PlayerInfo::GetGold()
{
	return gold;
}