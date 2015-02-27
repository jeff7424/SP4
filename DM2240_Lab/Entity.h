#pragma once

#include "Vector3.h"

class Entity {
private:
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	
public:
	Entity();
	~Entity();

	void SetPos(Vector3 pos);
	void SetVel(Vector3 vel);
	void SetScale(Vector3 scale);
	void SetActive(bool active);

	Vector3 GetPos();
	Vector3 GetVel();
	Vector3 GetScale();
	bool GetActive();

	void Update();
};