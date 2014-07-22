#pragma once

class Ship
{
public:
	Ship();
	Ship(std::string name, int hp);
	~Ship();

	void createShip(Ship shipData);
	bool HitCheck(Map ownMap);
	int GetHP(){ return hp; } // �ζ��� �Լ� :: �����ϰ� ���� ȣ��Ǵ� �Լ���
	void setPosition(Position setPos);
	

private:
	std::string name;
	
	int hp;
	bool direction; // 0 : ����, 1 : ����
	Position shipPosition;
	
};
