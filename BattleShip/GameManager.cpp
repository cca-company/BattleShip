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

void GameManager::StartGame()
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
		attackPos.x = MAP_WIDTH / 2;
		attackPos.y = MAP_HEIGHT / 2;
		if (attacker->GetType() == USER) m_Message = "please enter attack position";
		else m_Message = "waiting...";

		PrintWindow(attacker->GetName()+" turn");
		Sleep(2000);
		PrintWindow("");

		while (!attacker->Attack(&attackPos, &m_Message))
		{
			PrintWindow("");
		}
		hitState = defender->HitCheckMyShip(attackPos);

		if (hitState == MISS)
		{
			PrintWindow("MISS!");
		}
		else if (hitState == HIT)
		{
			PrintWindow("HIT!");
		}
		else
		{
			PrintWindow("DESTROY!");
		}
		Sleep(2000);

		// 공수교체
		temp = attacker;
		attacker = defender;
		defender = temp;
		m_PlayTurn++;

	}
}

// 게임 초기 정보를 세팅합니다.
void GameManager::InitGame()
{
	m_PlayTurn = 1;
	m_Message = "game start!";
	m_Player1->InitPlayer(USER);
	m_Player2->InitPlayer(AI);
}

// 게임 화면을 화면에 프린팅합니다.
void GameManager::PrintWindow(std::string notice)
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
	printf("\t\t\t\t       %02d \n\n", m_PlayTurn);

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
				printf_s("%c%c│", objMark[(objIdx + 2) * 2], objMark[(objIdx + 2) * 2 + 1]);
			}

			printf("\t");

			// Player2의 i번째 라인
			printf(" %c ", 'A' + i);
			printf("│");
			for (int k = 0; k < MAP_WIDTH; ++k)
			{
				objIdx = map2[i][k];
				printf_s("%c%c│", objMark[(objIdx + 2) * 2], objMark[(objIdx + 2) * 2 + 1]);

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
	











	/*
	printf("┌─┬─┬─┬─┬─┬─┬─┬─┐\n");
	printf("│  │  │  │  │  │  │  │  │\n");
	printf("├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("│  │  │  │  │  │  │  │  │\n");
	printf("├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("│  │  │  │  │  │  │  │  │\n");
	printf("├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("│  │  │  │  │  │  │  │  │\n");
	printf("├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("│  │  │  │  │  │  │  │  │\n");
	printf("├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("│  │  │  │  │  │  │  │  │\n");
	printf("├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("│  │  │  │  │  │  │  │  │\n");
	printf("├─┼─┼─┼─┼─┼─┼─┼─┤\n");
	printf("│  │  │  │  │  │  │  │  │\n");
	printf("└─┴─┴─┴─┴─┴─┴─┴─┘\n");
	*/

}