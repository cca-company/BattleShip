#include "stdafx.h"
#include "Map.h"


Map::Map()
{
}


Map::~Map()
{
}

void Map::InitMap()
{
	m_Width = MAP_WIDTH;
	m_Height = MAP_HEIGHT;
	memset(m_Field, 0, _countof(m_Field));
}

// Field �迭�� ������ ���� ������ �����մϴ�.
void Map::SetField(char posX, char posY, int shipIdx)
{
	m_Field[posY - 'A'][posX - '1'] = shipIdx;
}

// ������ ��ġ�� ����ִ��� �˻��մϴ�.
bool Map::CheckPosFull(char posX, char posY)
{
	if (m_Field[posY - 'A'][posX - '1'] != 0)
		return true;
	else
		return false;
}

// ������ ��ġ�� Ÿ�� ǥ���ϰ� ��ġ�ߴ� ���� idx�� �����մϴ�
int Map::HitMap(Position checkPos)
{
	//_ASSERTE(m_Field[checkPos.y][checkPos.x] < 0 && "Invalid position");

	int shipIdx = m_Field[checkPos.y-'A'][checkPos.x-'1'];
	m_Field[checkPos.y-'A'][checkPos.x-'1'] = (shipIdx <= 0) ? -1 : -2;

	return shipIdx;
}

void Map::GetField(int map[MAP_HEIGHT][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; ++i)
	{
		for (int j = 0; j < MAP_WIDTH; ++j)
		{
			map[i][j] = m_Field[i][j];
		}
	}
}