#pragma once

class Ship
{
public:
	Ship();
	~Ship();

	bool AddPosition(Position pos);
	HitResult HitCheck(Position hitPos);
	bool IsValidPos();
	void Print();

private:
	std::string	m_Name;
	Position m_Position[5];
	ShipType m_Type;
	int		 m_HP;
	int		 m_PosVal;
};
