#pragma once
#include "stdafx.h"

class Ship
{
public:
	/* initialize function */
	Ship();
	~Ship();
	void					InitShip();  // 게임 초기 정보를 세팅합니다
	bool					SetPos(char posX, char posY);  // 지정된 포지션을 세팅합니다

	/* activity function */
	void					PrintState(); // 자신의 상태를 출력합니다
	void					PrintMark(){ std::cout << m_Mark; }	// 화면에 배 마크를 출력합니다
		
	/* virtual function */
	virtual	HitState		GetHitState();  // Hit 결과를 리턴합니다

	/* state function */
	std::string				GetName()		{ return m_Name; }  // 배 이름을 리턴합니다
	ShipType				GetType()		{ return m_Type; }  // 배 타입을 리턴합니다
	int						GetHP()			{ return m_HP; }  // 배 현재 HP를 리턴합니다
	int						GetLength()		{ return m_Length; }  // 배 전체 길이를 리턴합니다


protected:
	ShipType				m_Type;
	std::string				m_Name;
	int						m_HP;
	int						m_Length;
	std::string				m_Mark;
	std::vector<Position>	m_Pos;
};