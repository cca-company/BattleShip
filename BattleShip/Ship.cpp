#include "stdafx.h"
#include "Ship.h"


Ship::Ship()
{
}


Ship::~Ship()
{
}

void Ship::InitShip()
{
	m_HP = m_Length;
	m_Pos.clear();
}

// 결정된 배의 Position을 세팅합니다.
bool Ship::SetPos(char posX, char posY)
{
	Position newPos;
	newPos.x = posX;
	newPos.y = posY;

	if (posX < '1' || posX >= '1' + MAP_WIDTH ||
		posY < 'A' || posY >= 'A' + MAP_HEIGHT)
		return false;

	if ((int)m_Pos.size() > m_HP) return false;

	m_Pos.push_back(newPos);
	return true;
}

// Hit시 상태를 출력합니다
HitState Ship::GetHitState()
{
	m_HP--;
	//_ASSERTE(m_HP >= 0 && "invalid HP");

	if (m_HP == 0)
		return DESTROY;
	else
		return HIT;
}

// 자신의 상태를 출력합니다
void Ship::PrintState()
{
	std::cout << m_Name << " ";
	for (auto& pos : m_Pos)
	{
		printf_s("%c%c ",pos.y, pos.x);
	}

	printf_s("\n");

}