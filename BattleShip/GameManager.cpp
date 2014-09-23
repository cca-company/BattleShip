#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager()
{
	// �� �÷��̾� ��� ������ �ʱ�ȭ�մϴ�.
	m_Player1 = new Player();
	m_Player2 = new Player();
}

GameManager::~GameManager()
{
	// �÷��̾� ��� ������ �޸𸮸� �����մϴ�.
	delete m_Player1;
	delete m_Player2;
}

// ���� �ʱ� ������ �����մϴ�.
void GameManager::InitGame(PlayerType playerType1, PlayerType playerType2)
{
	// ���� �ʱ�ȭ
	m_PlayTurn = 1;
	m_Render.SetMessage("game start!");
	srand((unsigned)time(NULL));

	// �÷��̾� �ʱ�ȭ
	m_Player1->InitPlayer(m_Render, playerType1);
	m_Player2->InitPlayer(m_Render, playerType2);

	// ������ �ʱ�ȭ
	m_Render.SetPlayerName(m_Player1->GetName(), m_Player2->GetName());
	
}

// ��Ʈ��ũ ���� �ʱ� ������ �����մϴ�
void GameManager::InitNetworkGame(Network network)
{
	int			map1[MAP_HEIGHT][MAP_WIDTH];
	int			mapValue;
	char		convertValue[] = { MD_NONE, MD_AIRCRAFT, MD_BATTLESHIP, MD_CRUISER, MD_DESTROYER1, MD_DESTROYER2 };
	char		mapData[MAP_HEIGHT*MAP_WIDTH];
	ErrorType	error;

	// Player1�� �ʱ�ȭ�ϰ� �� �����͸� �����մϴ�
	m_Player1->InitPlayer(m_Render, AI);
	m_Player1->GetPlayerMap(map1);

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			mapValue = map1[i][j];
			if (mapValue < 0) mapValue = 0;
			mapData[i*MAP_HEIGHT + j] = convertValue[mapValue];
		}
	}

	// �� ���� ����
	while (true)
	{
		error = network.SubmitMap(mapData);
		if (error == ET_INVALID_MAP)
			puts("��ȿ���� ���� �� �������Դϴ�.");
		else
			break;
	}
}

// ��Ʈ��ũ�� �����մϴ�
Network GameManager::ConnectNetwork()
{
	Network		network;

	srand((unsigned)time(NULL));

	try
	{
		Network::Initialize();
	}
	catch (Network::Exception)
	{
		printf("�ʱ�ȭ �� ������ �߻��߽��ϴ�.");
		return network;
	}

	try
	{
		network.Connect("10.73.42.117", 9001);
	}
	catch (Network::Exception ex)
	{
		switch (ex)
		{
		case Network::NETWORK_ERROR:
			puts("������ ���ῡ �����߽��ϴ�.");
			break;
		case Network::PARAMETER_ERROR:
			puts("�Լ��� �μ��� �߸��Ǿ����ϴ�.");
			break;
		}
		return network;
	}
	puts("���� ����!");

	try	// ���� ó���� ���� try������ ��� ���Ѵ�.
	{
		network.SubmitName(L"DongYu", 141040);

		Network::GameStartData gameStartData;
		puts("���� ���� �����");
		network.WaitForStart(&gameStartData);
		wprintf_s(L"��Ī�Ǿ����ϴ�. ���� �̸�: %s, �й�: %d\n", gameStartData.oppositionName, gameStartData.oppositionStudentID);


	}
	catch (Network::Exception ex)
	{
		switch (ex)
		{
		case Network::NETWORK_ERROR:
			puts("��Ʈ��ũ�� ������ �߻��߽��ϴ�.");
			break;
		case Network::SERVER_CLOSED:
			puts("�������� ������ ���������ϴ�.");
			break;
		case Network::PARAMETER_ERROR:
			puts("�Լ��� �μ��� �߸��Ǿ����ϴ�.");
			break;
		case Network::UNEXPECTED_PACKET:
			puts("�������� �߸��� ������ ���۵Ǿ����ϴ�.");
			break;
		default:
			break;
		}
	}

	return network;
}

// ���� �÷��̸� �����մϴ�.
void GameManager::StartGame()
{
	int menuNum;
	menuNum = m_Render.PrintStartScreen();

	// ������ �޴� ����
	switch (menuNum)
	{
	case 1:
		InitGame(AI, AI);
		StartSoloPlay();
		break;
	case 2:
		StartNetworkPlay();
		break;
	default:
		return;
		break;
	}
}

// �ַ��÷��̸� �����մϴ�
int GameManager::StartSoloPlay()
{
	Player			*attacker, *defender, *temp;
	Position		attackPos;
	HitState		hitState;
	int				tempMap1[MAP_HEIGHT][MAP_WIDTH];
	int				tempMap2[MAP_HEIGHT][MAP_WIDTH];
	std::string		hitNotice[] = {
		"Miss...",
		"Hit!",
		"Aircraft Destroy!!!",
		"Battleship Destroy!!!",
		"Cruiser Destroy!!!",
		"Destroyer Destroy!!!"
	};

	// ���� ���� ���� ����
	if (rand() % 2){
		attacker = m_Player1;
		defender = m_Player2;
	}
	else
	{
		attacker = m_Player2;
		defender = m_Player1;
	}

	while (m_Player1->GetLife() > 0 && m_Player2->GetLife() > 0)
	{
		// Ŀ�� ��ġ �� �޼��� �ʱ�ȭ
		attackPos.x = '1' + MAP_WIDTH / 2;
		attackPos.y = 'A' + MAP_HEIGHT / 2;
		m_Render.SetPlayTurn(m_PlayTurn);

		if (attacker->GetType() == USER) m_Render.SetMessage("������ ������ �����ϼ���");
		else m_Render.SetMessage("������ ���Դϴ�...");

		// ��� �� ����
		m_Player1->GetPlayerMap(tempMap1);
		m_Player2->GetPlayerMap(tempMap2);
		m_Render.SetMap(tempMap1, tempMap2);

		m_Render.PrintGameScreen(attackPos, attacker->GetName());

		// ���� ��ġ ����
		attackPos = attacker->Attack(m_Render);
		hitState = defender->HitCheckMyShip(attackPos);
		attacker->SetAttackResult(hitState);


		// ���� ��� ȭ�� ǥ��
		//m_Render.PrintGameScreen(hitNotice[hitState]);
		//Sleep(2000);

		// ������ü
		temp = attacker;
		attacker = defender;
		defender = temp;
		m_PlayTurn++;

	}
	
	m_Render.PrintGameScreen(defender->GetName()+" �¸�!");
	getchar();

	return (m_PlayTurn + 1) / 2;
}

// �¶��� �÷��̸� �����մϴ�.
int GameManager::StartNetworkPlay()
{
	Network		network = ConnectNetwork();
	PacketType	type;
	ErrorType	error;
	Position	attackPos;
	HitState	hitState;
	Coord		pos;
	HitState	ATTACK_RESULT[] = { DESTROY, MISS, HIT, DESTROY_AIRCRAFT, DESTROY_BATTLESHIP, DESTROY_CRUISER, DESTROY_DESTROYER };
	std::string ATTACK_RESULT_STR[] = { "Miss", "Hit", "Destroy", "Destroy Aircraft", "Destroy Battleship",
										"Destroy Cruiser", "Destroy Destroyer"};

	try	// ���� ó���� ���� try������ ��� ���Ѵ�.
	{
		bool allOver = false;
		
		while (!allOver)
		{
			InitNetworkGame(network);

			// ���� ���� ����
			bool gameOver = false;
			while (!gameOver)
			{
				error = network.GetPacketType(&type);

				switch (type)
				{
				// ������ �߻��ϴ� ���(������ ���� ����)
				case PKT_SC_ERROR:
					if (error == ET_OPPOSITION_DISCONNECTED)
						puts("������ ������ ���������ϴ�.");
					else
						puts("�� �� ���� �����Դϴ�.");
					return 0;

				// �� ����
				case PKT_SC_MY_TURN:
				{
					while (true)
					{
						// Ŀ�� ��ġ �� �޼��� �ʱ�ȭ
						attackPos.x = '1' + MAP_WIDTH / 2;
						attackPos.y = 'A' + MAP_HEIGHT / 2;

						// ���� ��ġ ����
						m_Player1->Attack(m_Render);
						pos.mX = attackPos.x-'1';
						pos.mY = attackPos.y-'A';

						error = network.SubmitAttack(pos);
						if (error == ET_INVALID_ATTACK)
							puts("��ȿ���� ���� ���� ��ġ�Դϴ�.");
						else
							break;
					}
					break;
				}

				// ���� ���
				case PKT_SC_ATTACK_RESULT:
				{
					Network::AttackResultData attackResult = network.GetAttackResult();
					if (attackResult.isMine)
					{
						puts("���� ���:");
						hitState = ATTACK_RESULT[attackResult.attackResult];
						m_Player1->SetAttackResult(hitState);
					}
					else
					{
						puts("�ǰ� ���:");
						attackPos.x = attackResult.pos.mX+'1';
						attackPos.y = attackResult.pos.mY+'A';
						hitState = m_Player1->HitCheckMyShip(attackPos);
					}
					printf_s("X: %c, Y: %c, RESULT: %s\n", attackPos.x, attackPos.y, ATTACK_RESULT_STR[hitState].c_str());
					break;
				}

				// ���� ����
				case PKT_SC_GAME_OVER:
				{
					Network::GameResultData gameResult = network.GetGameResult();
					if (gameResult.isWinner)
						puts("�¸�!!!");
					else
						puts("�й�...");
					printf_s("�� ��: %d\n", gameResult.turns);
					gameOver = true;
					break;
				}

				default:
					throw Network::UNEXPECTED_PACKET;
					break;
				}
			}

			// ������ ó��
			network.GetPacketType(&type);

			if (type == PKT_SC_NEXT_GAME)
			{
				puts("���� ������ �غ����ּ���.");
			}
			else if (type == PKT_SC_ALL_OVER)
			{
				Network::FinalResultData finalResult = network.GetFinalResult();
				puts("��� ����");
				printf_s("�¸� Ƚ��: %d, ��� �� ��: %.1f", finalResult.winCount, finalResult.avgTurns);

				allOver = true;
			}
			else
				throw Network::UNEXPECTED_PACKET;
		}
	}
	catch (Network::Exception ex)
	{
		switch (ex)
		{
		case Network::NETWORK_ERROR:
			puts("��Ʈ��ũ�� ������ �߻��߽��ϴ�.");
			break;
		case Network::SERVER_CLOSED:
			puts("�������� ������ ���������ϴ�.");
			break;
		case Network::PARAMETER_ERROR:
			puts("�Լ��� �μ��� �߸��Ǿ����ϴ�.");
			break;
		case Network::UNEXPECTED_PACKET:
			puts("�������� �߸��� ������ ���۵Ǿ����ϴ�.");
			break;
		default:
			break;
		}
	}
	return 0;
}
