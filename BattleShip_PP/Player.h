#pragma once

class Player
{
public:
	Player();
	~Player();
	void getMap(Position mapSize);
	void getShip(shipData);
	void allocateShip();
	void attack();


private:
	PLAYER_TYPE playerType; // 0 : ���, 1 : ��ǻ��, 2 : ��Ʈ��ũ
	std::string playerName;
	int level; // 0~2;
	Map ownMap;
	std::vector<Ship> ownShip;

};

