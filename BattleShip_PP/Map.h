#pragma once

class Map
{
public:
	Map();
	~Map();
	void createMap(Position mapSize);
	Position getMapSize();
	void hitPrint();

	
private:
	Position mapSize;
	int **mapHitArray; // mapSize 크기의 가로*세로를 가지는 2차원 배열, 0 : 정상, 1 : 피격

};

