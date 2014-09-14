#include "stdafx.h"


Ship::Ship()
{
	m_PosVal = 0;
	m_HP = 0;
	m_Type = SHIP_NONE;
}
Ship::~Ship()
{
}


bool Ship::AddPosition(Position pos)
{
	if (m_PosVal < m_HP)
	{
		m_Position[m_PosVal++] = pos;
	}
	
	if (m_PosVal == m_HP)
	{
		if (!IsValidPos())
		{
			m_PosVal = 0;
			return false;
		}
	}

	return true;
}

HitResult Ship::HitCheck(Position hitPos)
{

}

bool Ship::IsValidPos()
{
	bool direction; // 
	int min, max;
	direction = (m_Position[0].x == m_Position[1].x) ? true : false;
	// 방향 체크

	min = (direction) ? m_Position[0].y : m_Position[0].x;
	max = (direction) ? m_Position[0].y : m_Position[0].x;

	// 잘 배치되어있나 체크
	for (int i = 1; i < m_HP; i++)
	{
		if (direction)
		{
			if (min > m_Position[i].y) min = m_Position[i].y;
			if (max < m_Position[i].y) max = m_Position[i].y;
			if (m_Position[i].x != m_Position[0].x) return false;
		}
		else
		{
			if (min > m_Position[i].x) min = m_Position[i].x;
			if (max < m_Position[i].x) max = m_Position[i].x;
			if (m_Position[i].y != m_Position[0].y) return false;
		}
	}

	if (max-min != m_HP-1)
	{
		return false;
	}

	return true;
}

void Ship::Print()
{
	std::cout << m_Name << "is HP : " << m_HP;
}
