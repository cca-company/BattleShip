#pragma once
#include "stdafx.h"
#include "Player.h"

class GameManager
{
public:
	/* initialize function */
	GameManager();
	~GameManager();
	void				InitGame(); // 게임 초기 정보를 세팅합니다
	void				InitNetworkGame(Network& network); // 게임 초기 정보를 세팅합니다

	void				StartWindow(); // 시작화면을 출력합니다
	int					StartGame(); // 게임을 시작합니다
	int					StartNetworkGame(); // 온라인 플레이를 시작합니다

protected:
	void				PrintWindow(std::string notice,Position attackPos,Player* attacker); // 현재 게임 상태를 화면에 출력합니다
	void				HLight(std::string text);
	void				MkMapData(OUT char* mapData);

protected:
	Player*				m_Player1;
	Player*				m_Player2;
	int					m_PlayTurn;
	std::string			m_Message;
};