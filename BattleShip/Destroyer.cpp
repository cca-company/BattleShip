#include "stdafx.h"
#include "Destroyer.h"


Destroyer::Destroyer()
{
	m_Type = DESTROYER;
	m_Name = "Destroyer";
	m_Mark = "¨Ð";
	m_Length = 2;
}


Destroyer::~Destroyer()
{
}


HitState Destroyer::GetHitState()
{
	HitState curState = Ship::GetHitState();

	return (curState == DESTROY) ? DESTROY_DESTROYER : curState;

}
