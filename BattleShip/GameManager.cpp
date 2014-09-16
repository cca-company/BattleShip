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
		system("cls");
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
				case 2:StartGame();
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
	InitNetworkGame();
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

void GameManager::InitNetworkGame()
{
	Network		network;
	PacketType	type;
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
		network.Connect("127.0.0.1", 10000);
	}
	catch (Network::Exception ex)
	{
		switch ();
	}
	network.SubmitName(L"DongYu", 141040);

	Network::GameStartData gameStartData;
	network.WaitForStart(&gameStartData);

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