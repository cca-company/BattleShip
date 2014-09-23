#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager()
{
	// 각 플레이어 멤버 변수를 초기화합니다.
	m_Player1 = new Player();
	m_Player2 = new Player();
}

GameManager::~GameManager()
{
	// 플레이어 멤버 변수의 메모리를 해제합니다.
	delete m_Player1;
	delete m_Player2;
}

// 게임 초기 정보를 세팅합니다.
void GameManager::InitGame(PlayerType playerType1, PlayerType playerType2)
{
	// 변수 초기화
	m_PlayTurn = 1;
	m_Render.SetMessage("game start!");
	srand((unsigned)time(NULL));

	// 플레이어 초기화
	m_Player1->InitPlayer(m_Render, playerType1);
	m_Player2->InitPlayer(m_Render, playerType2);

	// 렌더러 초기화
	m_Render.SetPlayerName(m_Player1->GetName(), m_Player2->GetName());
	
}

// 네트워크 게임 초기 정보를 세팅합니다
void GameManager::InitNetworkGame(Network network)
{
	int			map1[MAP_HEIGHT][MAP_WIDTH];
	int			mapValue;
	char		convertValue[] = { MD_NONE, MD_AIRCRAFT, MD_BATTLESHIP, MD_CRUISER, MD_DESTROYER1, MD_DESTROYER2 };
	char		mapData[MAP_HEIGHT*MAP_WIDTH];
	ErrorType	error;

	// Player1을 초기화하고 맵 데이터를 전달합니다
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

	// 맵 정보 전달
	while (true)
	{
		error = network.SubmitMap(mapData);
		if (error == ET_INVALID_MAP)
			puts("유효하지 않은 맵 데이터입니다.");
		else
			break;
	}
}

// 네트워크에 연결합니다
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
		printf("초기화 중 문제가 발생했습니다.");
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
			puts("서버와 연결에 실패했습니다.");
			break;
		case Network::PARAMETER_ERROR:
			puts("함수의 인수가 잘못되었습니다.");
			break;
		}
		return network;
	}
	puts("접속 성공!");

	try	// 예외 처리를 위해 try문으로 모두 감싼다.
	{
		network.SubmitName(L"DongYu", 141040);

		Network::GameStartData gameStartData;
		puts("게임 시작 대기중");
		network.WaitForStart(&gameStartData);
		wprintf_s(L"매칭되었습니다. 상대방 이름: %s, 학번: %d\n", gameStartData.oppositionName, gameStartData.oppositionStudentID);


	}
	catch (Network::Exception ex)
	{
		switch (ex)
		{
		case Network::NETWORK_ERROR:
			puts("네트워크에 문제가 발생했습니다.");
			break;
		case Network::SERVER_CLOSED:
			puts("서버와의 연결이 끊어졌습니다.");
			break;
		case Network::PARAMETER_ERROR:
			puts("함수의 인수가 잘못되었습니다.");
			break;
		case Network::UNEXPECTED_PACKET:
			puts("서버에서 잘못된 정보가 전송되었습니다.");
			break;
		default:
			break;
		}
	}

	return network;
}

// 게임 플레이를 시작합니다.
void GameManager::StartGame()
{
	int menuNum;
	menuNum = m_Render.PrintStartScreen();

	// 선택한 메뉴 실행
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

// 솔로플레이를 시작합니다
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

	// 시작 순서 랜덤 결정
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
		// 커서 위치 및 메세지 초기화
		attackPos.x = '1' + MAP_WIDTH / 2;
		attackPos.y = 'A' + MAP_HEIGHT / 2;
		m_Render.SetPlayTurn(m_PlayTurn);

		if (attacker->GetType() == USER) m_Render.SetMessage("공격할 지점을 선택하세요");
		else m_Render.SetMessage("상대방의 턴입니다...");

		// 출력 맵 세팅
		m_Player1->GetPlayerMap(tempMap1);
		m_Player2->GetPlayerMap(tempMap2);
		m_Render.SetMap(tempMap1, tempMap2);

		m_Render.PrintGameScreen(attackPos, attacker->GetName());

		// 공격 위치 선택
		attackPos = attacker->Attack(m_Render);
		hitState = defender->HitCheckMyShip(attackPos);
		attacker->SetAttackResult(hitState);


		// 공격 결과 화면 표시
		//m_Render.PrintGameScreen(hitNotice[hitState]);
		//Sleep(2000);

		// 공수교체
		temp = attacker;
		attacker = defender;
		defender = temp;
		m_PlayTurn++;

	}
	
	m_Render.PrintGameScreen(defender->GetName()+" 승리!");
	getchar();

	return (m_PlayTurn + 1) / 2;
}

// 온라인 플레이를 시작합니다.
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

	try	// 예외 처리를 위해 try문으로 모두 감싼다.
	{
		bool allOver = false;
		
		while (!allOver)
		{
			InitNetworkGame(network);

			// 게임 루프 시작
			bool gameOver = false;
			while (!gameOver)
			{
				error = network.GetPacketType(&type);

				switch (type)
				{
				// 에러가 발생하는 경우(상대방의 접속 종료)
				case PKT_SC_ERROR:
					if (error == ET_OPPOSITION_DISCONNECTED)
						puts("상대방의 접속이 끊어졌습니다.");
					else
						puts("알 수 없는 에러입니다.");
					return 0;

				// 내 차례
				case PKT_SC_MY_TURN:
				{
					while (true)
					{
						// 커서 위치 및 메세지 초기화
						attackPos.x = '1' + MAP_WIDTH / 2;
						attackPos.y = 'A' + MAP_HEIGHT / 2;

						// 공격 위치 선택
						m_Player1->Attack(m_Render);
						pos.mX = attackPos.x-'1';
						pos.mY = attackPos.y-'A';

						error = network.SubmitAttack(pos);
						if (error == ET_INVALID_ATTACK)
							puts("유효하지 않은 공격 위치입니다.");
						else
							break;
					}
					break;
				}

				// 공격 결과
				case PKT_SC_ATTACK_RESULT:
				{
					Network::AttackResultData attackResult = network.GetAttackResult();
					if (attackResult.isMine)
					{
						puts("공격 결과:");
						hitState = ATTACK_RESULT[attackResult.attackResult];
						m_Player1->SetAttackResult(hitState);
					}
					else
					{
						puts("피격 결과:");
						attackPos.x = attackResult.pos.mX+'1';
						attackPos.y = attackResult.pos.mY+'A';
						hitState = m_Player1->HitCheckMyShip(attackPos);
					}
					printf_s("X: %c, Y: %c, RESULT: %s\n", attackPos.x, attackPos.y, ATTACK_RESULT_STR[hitState].c_str());
					break;
				}

				// 게임 종료
				case PKT_SC_GAME_OVER:
				{
					Network::GameResultData gameResult = network.GetGameResult();
					if (gameResult.isWinner)
						puts("승리!!!");
					else
						puts("패배...");
					printf_s("턴 수: %d\n", gameResult.turns);
					gameOver = true;
					break;
				}

				default:
					throw Network::UNEXPECTED_PACKET;
					break;
				}
			}

			// 종료후 처리
			network.GetPacketType(&type);

			if (type == PKT_SC_NEXT_GAME)
			{
				puts("다음 게임을 준비해주세요.");
			}
			else if (type == PKT_SC_ALL_OVER)
			{
				Network::FinalResultData finalResult = network.GetFinalResult();
				puts("모두 종료");
				printf_s("승리 횟수: %d, 평균 턴 수: %.1f", finalResult.winCount, finalResult.avgTurns);

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
			puts("네트워크에 문제가 발생했습니다.");
			break;
		case Network::SERVER_CLOSED:
			puts("서버와의 연결이 끊어졌습니다.");
			break;
		case Network::PARAMETER_ERROR:
			puts("함수의 인수가 잘못되었습니다.");
			break;
		case Network::UNEXPECTED_PACKET:
			puts("서버에서 잘못된 정보가 전송되었습니다.");
			break;
		default:
			break;
		}
	}
	return 0;
}
