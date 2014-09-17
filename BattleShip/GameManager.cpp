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


// 게임 시작 화면을 프린팅합니다.
void GameManager::StartWindow()
{
	int cursor = 1;
	while (true)
	{
		//system("cls");
		printf("BATTLESHIP\n");
		if (cursor == 1) HLight("* SOLO PLAY *\n");
		else printf("* SOLO PLAY *\n");
		if (cursor == 2) HLight("*  NETWORK  *\n");
		else printf("*  NETWORK  *\n");
		if (cursor == 3) HLight("*    END    *\n");
		else printf("*    END    *\n");

		while (true)
		{
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				if (cursor<3) cursor++;
				break;
			}
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				if (cursor>1) cursor--;
				break;
			}
			if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			{
				switch (cursor)
				{
				case 1:StartGame();
					break;
				case 2:StartNetworkGame();
					break;
				case 3:return;
					break;
				}
				break;
			}
			Sleep(50);
		}
	}

}

// 게임 플레이를 시작합니다.
int GameManager::StartGame()
{
	InitGame();

	Player *attacker, *defender, *temp;
	Position attackPos;
	HitState hitState;

	// 시작 순서 랜덤 설정
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
		attackPos.x = '1'+MAP_WIDTH / 2;
		attackPos.y = 'A'+MAP_HEIGHT / 2;
		if (attacker->GetType() == USER) m_Message = "공격할 지점을 선택하세요";
		else m_Message = "상대방의 턴입니다...";

		
		//PrintWindow(attacker->GetName()+" turn", attackPos, attacker);
		//Sleep(2000);
		//PrintWindow("", attackPos,attacker);
		

		// 공격 위치 선택
		while (!attacker->Attack(&attackPos, &m_Message))
		{
			//if(attacker->GetType() == USER) PrintWindow("", attackPos, attacker);
		}
		hitState = defender->HitCheckMyShip(attackPos);
		attacker->SetPrevState(hitState);

		
		// 공격 결과 화면 표시
		/*
		switch (hitState)
		{
		case MISS:
			PrintWindow("Miss...", attackPos, attacker);
			break;
		case HIT:
			PrintWindow("Hit!", attackPos, attacker);
			break;
		case DESTROY_AIRCRAFT:
			PrintWindow("Aircraft Destroy!!!", attackPos, attacker);
			break;
		case DESTROY_BATTLESHIP:
			PrintWindow("Battleship Destroy!!!", attackPos, attacker);
			break;
		case DESTROY_CRUISER:
			PrintWindow("Cruiser Destroy!!!", attackPos, attacker);
			break;
		case DESTROY_DESTROYER:
			PrintWindow("Destroyer Destroy!!!", attackPos, attacker);
			break;
		default:
			break;
		}
		Sleep(2000);
		*/
		
		// 공수교체
		temp = attacker;
		attacker = defender;
		defender = temp;
		m_PlayTurn++;

	}

	return (m_PlayTurn + 1) / 2;
}

// 온라인 플레이를 시작합니다.
int GameManager::StartNetworkGame()
{
	Network		network;
	PacketType	type;
	ErrorType	error;
	Position	attackPos;
	HitState	hitState;
	Coord		pos;
	HitState	ATTACK_RESULT[] = { DESTROY, MISS, HIT, DESTROY_AIRCRAFT, DESTROY_BATTLESHIP, DESTROY_CRUISER, DESTROY_DESTROYER };
	std::string ATTACK_RESULT_STR[] = { "Miss", "Hit", "Destroy", "Destroy Aircraft", "Destroy Battleship",
										"Destroy Cruiser", "Destroy Destroyer"};

	char		mapData[MAP_SIZE];

	try	// 예외 처리를 위해 try문으로 모두 감싼다.
	{
		InitNetworkGame(network);
		
		bool allOver = false;
		
		while (!allOver)
		{
			m_Player1->InitPlayer(AI);
			// 배 배치
			while (true)
			{
				MkMapData(mapData);

				error = network.SubmitMap(mapData);
				if (error == ET_INVALID_MAP)
					puts("유효하지 않은 맵 데이터입니다.");
				else
					break;
			}

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
					/*
					** 공격 위치 전송
					x, y는 0~7 사이의 정수이다.
					*/
					while (true)
					{
						// 자신의 공격 위치 제작 함수를 사용한다.

						// 커서 위치 및 메세지 초기화
						attackPos.x = '1' + MAP_WIDTH / 2;
						attackPos.y = 'A' + MAP_HEIGHT / 2;

						// 공격 위치 선택
						while (!m_Player1->Attack(&attackPos, &m_Message));
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
						// 자신의 공격 결과 처리 함수를 사용한다.
						hitState = ATTACK_RESULT[attackResult.attackResult];
						m_Player1->SetPrevState(hitState);
					}
					else
					{
						puts("피격 결과:");
						// 자신의 공격 결과 처리 함수를 사용한다.
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

			/*
			** 종료후 처리
			*/
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
}

void GameManager::MkMapData(OUT char* mapData)
{
	int map1[MAP_HEIGHT][MAP_WIDTH];
	int	mapValue;
	char convertValue[] = { MD_NONE, MD_AIRCRAFT, MD_BATTLESHIP, MD_CRUISER, MD_DESTROYER1, MD_DESTROYER2 };

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
}


// 게임 초기 정보를 세팅합니다.
void GameManager::InitGame()
{
	// 초기 변수 설정
	m_PlayTurn = 1;
	m_Message = "game start!";
	srand((unsigned)time(NULL));

	m_Player1->InitPlayer(AI);
	m_Player2->InitPlayer(AI);
}

void GameManager::InitNetworkGame(Network& network)
{
	ErrorType	error;

	srand((unsigned)time(NULL));

	try
	{
		Network::Initialize();
	}
	catch(Network::Exception)
	{
		printf("초기화 중 문제가 발생했습니다.");
		return;
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
		return;
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

}

/* 렌더러 함수 */

// 게임 화면을 화면에 프린팅합니다.
void GameManager::PrintWindow(std::string notice, Position attackPos, Player* attacker)
{
	int map1[MAP_HEIGHT][MAP_WIDTH];
	int map2[MAP_HEIGHT][MAP_WIDTH];
	char* objMark = "★☆  ⓐⓑⓒⓓⓓ";
	int	objIdx;

	m_Player1->GetPlayerMap(map1);
	m_Player2->GetPlayerMap(map2);

	system("cls");

	// 현재 턴수 출력

	printf("\n\t\t\t\t    * Turn * \n");
	printf("\t\t\t\t       %02d \n\n", (m_PlayTurn+1)/2);

	// 메세지 출력
	for (unsigned int i = 0; i < (76-m_Message.size())/2; ++i)
	{
		printf(" ");
	}
	printf("[ %s ]\n\n", m_Message.c_str());

	// 맵 출력
	printf("   ");
	for (int i = 0; i < MAP_WIDTH; ++i) printf("   %c", '1' + i);
	printf("\t   ");
	for (int i = 0; i < MAP_WIDTH; ++i) printf("   %c", '1' + i);
	printf("\n");

	printf("   ┌─┬─┬─┬─┬─┬─┬─┬─┐");
	printf("\t");
	printf("   ┌─┬─┬─┬─┬─┬─┬─┬─┐\n");


	for (int i = 0; i < MAP_HEIGHT; ++i)
	{
		// notice 존재시 2~7라인에 notice 출력
		if (notice != "" && i>=2 && i<=4)
		{
			if (i == 2)
			{
				printf("-------------------------------------------------------------------------------\n\n");
				for (unsigned int j = 0; j < (80 - notice.size()) / 2; ++j)
				{
					printf(" ");
				}
				printf("%s\n", notice.c_str());
				printf("\n-------------------------------------------------------------------------------\n");
				printf("   ├─┼─┼─┼─┼─┼─┼─┼─┤");
				printf("\t");
				printf("   ├─┼─┼─┼─┼─┼─┼─┼─┤");
				printf("\n");
			}

		}
		else
		{

			// Player1의 i번째 라인 맵 출력
			printf(" %c ",'A'+i);
			printf("│");
			for (int j = 0; j < MAP_WIDTH; ++j)
			{
				objIdx = map1[i][j];
				if (attackPos.x == '1' + j && attackPos.y == 'A'+i &&
					attacker != m_Player1)
				{
					printf_s("■│");
				}
				else
				{
					printf_s("%c%c│", objMark[(objIdx + 2) * 2], objMark[(objIdx + 2) * 2 + 1]);
				}
			}

			printf("\t");

			// Player2의 i번째 라인
			printf(" %c ", 'A' + i);
			printf("│");
			for (int k = 0; k < MAP_WIDTH; ++k)
			{
				objIdx = map2[i][k];
				if (attackPos.x == '1' + k && attackPos.y == 'A' + i &&
					attacker != m_Player2)
				{
					printf_s("■│");
				}
				else
				{
					printf_s("%c%c│", objMark[(objIdx + 2) * 2], objMark[(objIdx + 2) * 2 + 1]);
				}

			}
			printf("\n");

			// 하단라인
			if (i < MAP_HEIGHT-1){
				printf("   ├─┼─┼─┼─┼─┼─┼─┼─┤");
				printf("\t");
				printf("   ├─┼─┼─┼─┼─┼─┼─┼─┤");
				printf("\n");
			}

		}

	}

	printf("   └─┴─┴─┴─┴─┴─┴─┴─┘");
	printf("\t");
	printf("   └─┴─┴─┴─┴─┴─┴─┴─┘\n");

	// 게이머 이름 출력
	printf("────────┐");
	printf("\t\t\t\t\t     ");
	printf("┌────────\n");


	printf("% 10s\t│", m_Player1->GetName().c_str());
	printf("\t\t\t\t\t     ");
	printf("│   % 10s\n", m_Player2->GetName().c_str());

	printf("────────┘");
	printf("\t\t\t\t\t     ");
	printf("└────────\n");

	// 오브젝트 상태 출력
	for (int i = 0; i < m_Player1->GetShipNum(); ++i)
	{
		m_Player1->PrintMyShip(i,false);
		printf("\t\t\t    ");
		m_Player2->PrintMyShip(i,true);
		printf("\n");
	}

}

// 텍스트를 하이라이팅합니다
void GameManager::HLight(std::string text)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0070);
	std::cout << text;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0007);

}