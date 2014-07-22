#pragma once

class Ship
{
public:
	Ship();
	Ship(std::string name, int hp);
	~Ship();

	void createShip(Ship shipData);
	bool HitCheck(Map ownMap);
	int GetHP(){ return hp; } // 인라인 함수 :: 간단하고 자주 호출되는 함수들
	void setPosition(Position setPos);
	

private:
	std::string name;
	
	int hp;
	bool direction; // 0 : 가로, 1 : 세로
	Position shipPosition;
	
};
