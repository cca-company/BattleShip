#include "stdafx.h"
#include "Aircraft.h"


Aircraft::Aircraft()
{
	m_Type = AIRCRAFT;
	m_Name = "Aircraft";
	m_Mark = "¨Í";
	m_Length = 5;
}


Aircraft::~Aircraft()
{
}


HitState Aircraft::GetHitState()
{
	HitState curState = Ship::GetHitState();

	return (curState == DESTROY) ? DESTROY_AIRCRAFT : curState;

}
