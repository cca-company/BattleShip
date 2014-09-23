#pragma once
#include "stdafx.h"
#include "Ship.h"
#include "Aircraft.h"
#include "Battleship.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Map.h"
#include "Render.h"

class Player
{
public:
	/* initialize function */
	Player();
	~Player();
	void					InitPlayer(Render render, PlayerType type);  // ���� �ʱ� ������ �����մϴ�
	
	/* activity function */
	Position				Attack(Render render);  // ���� ��ġ�� �����մϴ�
	HitState				HitCheckMyShip(Position attackPos);  // ��� ���ݿ� ���� ���ظ� üũ�մϴ�

	/* state function */
	void					SetAttackResult(HitState state);
	void					GetPlayerMap(OUT int map[MAP_HEIGHT][MAP_WIDTH]);

	int						GetLife()		{ return m_Life; }
	int						GetShipNum()	{ return m_ShipList.size(); }
	std::string				GetName()		{ return m_Name; }
	PlayerType				GetType()		{ return m_Type; }

protected:
	void					SetShip(Render render);  // �ڽ��� ���� �踦 ��ġ�մϴ�
	bool					IsValidSetPos(char setX, char setY, Direction setDir, int shipHP);  // ���� ��ġ�� �� ��ġ�� ��ȿ�Ѱ� �˻��մϴ�
	Position				AISelectPos();

protected:
	PlayerType				m_Type;
	std::string				m_Name;
	int						m_Life;
	Direction				m_PredictDir;
	Position				m_PredictPos;
	std::vector <Position>	m_AttackedPos;

	Map						m_Map;
	std::vector <Ship*>		m_ShipList;
};

