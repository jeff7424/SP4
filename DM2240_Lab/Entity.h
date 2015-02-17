#pragma once

#include "Vector3.h"

class Entity {
private:
	Vector3 pos;
	Vector3 vel;	
	bool active;
	
public:
	Entity();
	~Entity();

	void SetPos(Vector3 pos);
	void SetVel(Vector3 vel);
	void SetActive(bool active);

	Vector3 GetPos();
	Vector3 GetVel();
	bool GetActive();

	void Update();
};