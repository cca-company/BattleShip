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
	int **mapHitArray; // mapSize ũ���� ����*���θ� ������ 2���� �迭, 0 : ����, 1 : �ǰ�

};

