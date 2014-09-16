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
	void					InitPlayer(PlayerType type);  // ���� �ʱ� ������ �����մϴ�
	
	/* activity function */
	bool					Attack(OUT Position* attackPos, OUT std::string* messageBox);  // ���� ��ġ�� �����մϴ�
	HitState				HitCheckMyShip(Position attackPos);  // ��� ���ݿ� ���� ���ظ� üũ�մϴ�
	void					PrintMyShip(int shipIdx, bool inverseOp);

	/* state function */
	int						GetLife(){ return m_Life; }
	int						GetShipNum(){ return m_ShipList.size(); }
	void					GetPlayerMap(OUT int map[MAP_HEIGHT][MAP_WIDTH]);
	std::string				GetName(){ return m_Name; }
	PlayerType				GetType(){ return m_Type; }
	void					SetPrevState(HitState state){ m_PrevState = state; }

protected:
	void					SetShip();  // �ڽ��� ���� �踦 ��ġ�մϴ�
	bool					IsValidSetPos(char setX, char setY, Direction setDir, int shipHP);  // ���� ��ġ�� �� ��ġ�� ��ȿ�Ѱ� �˻��մϴ�


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

