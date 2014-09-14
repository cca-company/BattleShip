#include "stdafx.h"
#include "Battleship.h"


Battleship::Battleship()
{
	m_Type = BATTLESHIP;
	m_Name = "Battleship";
	m_Mark = "¨Î";
	m_Length = 4;
}


Battleship::~Battleship()
{
}


HitState Battleship::GetHitState()
{
	HitState curState = Ship::GetHitState();

	return (curState == DESTROY) ? DESTROY_BATTLESHIP : curState;

}
