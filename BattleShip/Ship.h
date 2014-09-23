#pragma once
#include "stdafx.h"

class Ship
{
public:
	/* initialize function */
	Ship();
	~Ship();
	void					InitShip();  // ���� �ʱ� ������ �����մϴ�
	bool					SetPos(char posX, char posY);  // ������ �������� �����մϴ�

	/* activity function */
	void					PrintState(); // �ڽ��� ���¸� ����մϴ�
	void					PrintMark(){ std::cout << m_Mark; }	// ȭ�鿡 �� ��ũ�� ����մϴ�
		
	/* virtual function */
	virtual	HitState		GetHitState();  // Hit ����� �����մϴ�

	/* state function */
	std::string				GetName()		{ return m_Name; }  // �� �̸��� �����մϴ�
	ShipType				GetType()		{ return m_Type; }  // �� Ÿ���� �����մϴ�
	int						GetHP()			{ return m_HP; }  // �� ���� HP�� �����մϴ�
	int						GetLength()		{ return m_Length; }  // �� ��ü ���̸� �����մϴ�


protected:
	ShipType				m_Type;
	std::string				m_Name;
	int						m_HP;
	int						m_Length;
	std::string				m_Mark;
	std::vector<Position>	m_Pos;
};