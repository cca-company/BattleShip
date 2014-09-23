#pragma once
#include "stdafx.h"
#include "Player.h"
#include "Render.h"

class GameManager
{
public:
	/* initialize function */
	GameManager();
	~GameManager();

	void				StartGame(); // 게임을 시작합니다
	int					StartSoloPlay(); // 온라인 플레이를 시작합니다
	int					StartNetworkPlay(); // 온라인 플레이를 시작합니다

protected:
	Network				ConnectNetwork(); // 온라인 플레이를 위해 네트워크에 연결합니다
	void				InitGame(PlayerType playerType1, PlayerType playerType2); // 게임 정보를 초기화합니다
	void				InitNetworkGame(Network network); // 네트워크 게임 정보를 초기화합니다

protected:
	Player*				m_Player1;
	Player*				m_Player2;
	Render				m_Render;
	int					m_PlayTurn;
};