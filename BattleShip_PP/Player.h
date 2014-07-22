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
	PLAYER_TYPE playerType; // 0 : 사람, 1 : 컴퓨터, 2 : 네트워크
	std::string playerName;
	int level; // 0~2;
	Map ownMap;
	std::vector<Ship> ownShip;

};

