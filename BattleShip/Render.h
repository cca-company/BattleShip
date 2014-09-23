#pragma once
#include "stdafx.h"

class Render
{
public:
	Render();
	~Render();

	/* activity function */
	int					PrintStartScreen(); // ������ ����ȭ�� ���
	std::string			PrintInputNameScreen(); // �̸� ����ȭ�� ���
	void				PrintSettingScreen(OUT Position& setPos, OUT Direction& setDir, int shipHP); // ����ȭ�� ���
	void				PrintGameScreen(Position attackPos, std::string attackerName); // ���� ���� ȭ�� ���
	void				PrintGameScreen(std::string notice); // notice ȭ�� ��� �������̵�

	/* state function */
	void				SetMap(int mapData1[MAP_HEIGHT][MAP_WIDTH], int mapData2[MAP_HEIGHT][MAP_WIDTH]); // �� ���� ����
	void				SetMap(int mapData1[MAP_HEIGHT][MAP_WIDTH]); // �� ���� ����
	void				SetPlayerName(std::string playerName1, std::string playerName2); // �÷��̾� �̸� ����

	void				SetMessage(std::string message)		{ m_Message = message; } // ��� �޼��� ����
	void				SetPlayTurn(int playTurn)			{ m_PlayTurn = playTurn; }
	void				SetMapViewOp(bool op1, bool op2)	{ m_MapView1 = op1; m_MapView2 = op2; }
		
protected:
	void				HLight(std::string text); // �ؽ�Ʈ�� ���̶���Ʈ ó���� �մϴ�

protected:
	std::string			m_Message;
	int					m_Map1[MAP_HEIGHT][MAP_WIDTH];
	int					m_Map2[MAP_HEIGHT][MAP_WIDTH];
	std::string			m_PlayerName1;
	std::string			m_PlayerName2;
	bool				m_MapView1;
	bool				m_MapView2;
	char*				m_ObjMark = "�ڡ�  �ͨΨϨШ�";
	int					m_PlayTurn;

};

