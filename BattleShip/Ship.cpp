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

// ������ ���� Position�� �����մϴ�.
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

// Hit�� ���¸� ����մϴ�
HitState Ship::GetHitState()
{
	m_HP--;
	//_ASSERTE(m_HP >= 0 && "invalid HP");

	if (m_HP == 0)
		return DESTROY;
	else
		return HIT;
}

// �ڽ��� ���¸� ����մϴ�
void Ship::PrintState()
{
	std::cout << m_Name << " ";
	for (auto& pos : m_Pos)
	{
		printf_s("%c%c ",pos.y, pos.x);
	}

	printf_s("\n");

}