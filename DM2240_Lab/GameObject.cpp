
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	color(1, 1, 1),
	health(0),
	damage(0)
{
}

GameObject::~GameObject()
{
}