#pragma once
#include "stdafx.h"
#include "Player.h"

class GameManager
{
public:
	/* initialize function */
	GameManager();
	~GameManager();
	void				InitGame(); // ���� �ʱ� ������ �����մϴ�
	void				InitNetworkGame(Network& network); // ���� �ʱ� ������ �����մϴ�

	void				StartWindow(); // ����ȭ���� ����մϴ�
	int					StartGame(); // ������ �����մϴ�
	int					StartNetworkGame(); // �¶��� �÷��̸� �����մϴ�

protected:
	void				PrintWindow(std::string notice,Position attackPos,Player* attacker); // ���� ���� ���¸� ȭ�鿡 ����մϴ�
	void				HLight(std::string text);
	void				MkMapData(OUT char* mapData);

protected:
	Player*				m_Player1;
	Player*				m_Player2;
	int					m_PlayTurn;
	std::string			m_Message;
};