#pragma once

class Player
{
public:
	Player();
	~Player();
	void GetMap(Position mapSize);
	void GetShip(shipData);
	void AllocateShip();
	void Attack();


private:
	PLAYER_TYPE m_PlayerType; // 0 : ���, 1 : ��ǻ��, 2 : ��Ʈ��ũ
	std::string m_PlayerName;
	int m_Level; // 0~2;
	Map m_OwnMap;
	std::vector<Ship> m_OwnShip;
};

