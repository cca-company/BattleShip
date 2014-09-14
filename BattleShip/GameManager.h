#pragma once
#include "stdafx.h"
#include "Player.h"

class GameManager
{
public:
	// initialize function
	GameManager();
	~GameManager();
	void InitGame(); // ���� �ʱ� ������ �����մϴ�

	void StartGame(); // ������ �����մϴ�

protected:
	void PrintWindow(std::string notice); // ���� ���� ���¸� ȭ�鿡 ����մϴ�

protected:
	Player*	m_Player1;
	Player*	m_Player2;
	int		m_PlayTurn;
	std::string m_Message;
};

