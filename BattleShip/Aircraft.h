#pragma once
#include "Ship.h"
class Aircraft :
	public Ship
{
public:
	Aircraft();
	~Aircraft();
	HitState GetHitState();

};

