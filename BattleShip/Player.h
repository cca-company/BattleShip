#pragma once
#include "stdafx.h"
#include "Ship.h"
#include "Aircraft.h"
#include "Battleship.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Map.h"

class Player
{
public:
	/* initialize function */
	Player();
	~Player();
	void					InitPlayer(PlayerType type);  // 게임 초기 정보를 세팅합니다
	
	/* activity function */
	bool					Attack(OUT Position* attackPos, OUT std::string* messageBox);  // 공격 위치를 지정합니다
	HitState				HitCheckMyShip(Position attackPos);  // 상대 공격에 입은 피해를 체크합니다
	void					PrintMyShip(int shipIdx, bool inverseOp);

	/* state function */
	int						GetLife(){ return m_Life; }
	int						GetShipNum(){ return m_ShipList.size(); }
	void					GetPlayerMap(OUT int map[MAP_HEIGHT][MAP_WIDTH]);
	std::string				GetName(){ return m_Name; }
	PlayerType				GetType(){ return m_Type; }
	void					SetPrevState(HitState state){ m_PrevState = state; }

protected:
	void					SetShip();  // 자신이 가진 배를 배치합니다
	bool					IsValidSetPos(char setX, char setY, Direction setDir, int shipHP);  // 랜덤 배치한 배 위치가 유효한가 검사합니다


protected:
	PlayerType				m_Type;
	std::string				m_Name;
	int						m_Life;
	HitState				m_PrevState;
	Direction				m_PredictDir;
	Position				m_PredictPos;
	std::vector <Position>	m_AttackedPos;

	Map						m_Map;
	std::vector <Ship*>		m_ShipList;
};

