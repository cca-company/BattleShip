#pragma once
#include "stdafx.h"
#include "Player.h"

class GameManager
{
public:
	// initialize function
	GameManager();
	~GameManager();
	void InitGame(); // 게임 초기 정보를 세팅합니다

	void StartGame(); // 게임을 시작합니다

protected:
	void PrintWindow(std::string notice); // 현재 게임 상태를 화면에 출력합니다

protected:
	Player*	m_Player1;
	Player*	m_Player2;
	int		m_PlayTurn;
	std::string m_Message;
};

