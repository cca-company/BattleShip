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

	void				StartGame(); // ������ �����մϴ�
	int					StartSoloPlay(); // �¶��� �÷��̸� �����մϴ�
	int					StartNetworkPlay(); // �¶��� �÷��̸� �����մϴ�

protected:
	Network				ConnectNetwork(); // �¶��� �÷��̸� ���� ��Ʈ��ũ�� �����մϴ�
	void				InitGame(PlayerType playerType1, PlayerType playerType2); // ���� ������ �ʱ�ȭ�մϴ�
	void				InitNetworkGame(Network network); // ��Ʈ��ũ ���� ������ �ʱ�ȭ�մϴ�

protected:
	Player*				m_Player1;
	Player*				m_Player2;
	Render				m_Render;
	int					m_PlayTurn;
};