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

// Field 배열에 지정된 배의 정보를 세팅합니다.
void Map::SetField(char posX, char posY, int shipIdx)
{
	m_Field[posY - 'A'][posX - '1'] = shipIdx;
}

// 지정한 위치가 비어있는지 검사합니다.
bool Map::CheckPosFull(char posX, char posY)
{
	if (m_Field[posY - 'A'][posX - '1'] != 0)
		return true;
	else
		return false;
}

// 지정한 위치에 타격 표시하고 위치했던 배의 idx를 리턴합니다
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