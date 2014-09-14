#include "stdafx.h"
#include "Cruiser.h"


Cruiser::Cruiser()
{
	m_Type = CRUISER;
	m_Name = "Cruiser";
	m_Mark = "¨Ï";
	m_Length = 3;
}


Cruiser::~Cruiser()
{
}


HitState Cruiser::GetHitState()
{
	HitState curState = Ship::GetHitState();

	return (curState == DESTROY) ? DESTROY_CRUISER : curState;

}
