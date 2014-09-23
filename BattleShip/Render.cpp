#include "stdafx.h"
#include "Render.h"


Render::Render()
{
	m_MapView1 = true;
	m_MapView2 = true;
	m_PlayTurn = 0;

	memset(m_Map1, 0, sizeof(int) * MAP_HEIGHT * MAP_WIDTH);
	memset(m_Map2, 0, sizeof(int) * MAP_HEIGHT * MAP_WIDTH);
}


Render::~Render()
{
}

// 게임의 시작화면 출력
int Render::PrintStartScreen()
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
				return cursor;
			}
			Sleep(50);
		}
	}
}

// 이름 설정화면 출력
std::string Render::PrintInputNameScreen()
{
	std::string temp;
	std::string result;

	system("cls");
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	std::cout << "\n\n\n\n\n\n\n\n\t\t\t  이름을 입력하세요(최대 10자)\n\t\t\t   : ";
	std::cin >> temp;

	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');

	result.assign(temp, 0, 10);

	return result;
}

// 배 위치 설정화면 출력
void Render::PrintSettingScreen(OUT Position& setPos, OUT Direction& setDir, int shipHP)
{
	Position		cursor;
	int				objIdx;
	int				tempHP;
	bool			isSelected = false;

	setDir = DOWN;
	setPos.x = '1' + MAP_WIDTH / 2 - 1;
	setPos.y = 'A' + MAP_HEIGHT / 2 - 1;

	// do-while :: 맵 출력
	do{
		cursor = setPos;
		tempHP = shipHP;
		system("cls");

		printf("\n\n\n\t\t\t     배를 배치할 위치를 선택하세요\n\n");

		printf("\t\t\t   ");
		for (int i = 0; i < MAP_WIDTH; ++i) printf("   %c", '1' + i);

		printf("\n\t\t\t   ┌─┬─┬─┬─┬─┬─┬─┬─┐\n");

		for (int i = 0; i < MAP_HEIGHT; ++i)
		{
			// Player1의 i번째 라인 맵 출력
			printf("\t\t\t %c ", 'A' + i);
			printf("│");
			for (int j = 0; j < MAP_WIDTH; ++j)
			{
				objIdx = m_Map1[i][j];
				if (cursor.x == '1' + j && cursor.y == 'A' + i)
				{
					printf_s("■│");
					if (tempHP > 1 && setDir == RIGHT) cursor.x++;
					else if (tempHP > 1) cursor.y++;
					--tempHP;
				}
				else
				{
					printf_s("%c%c│", m_ObjMark[(objIdx + 2) * 2], m_ObjMark[(objIdx + 2) * 2 + 1]);
				}
			}

			printf("\n");

			// 하단라인
			if (i < MAP_HEIGHT - 1) printf("\t\t\t   ├─┼─┼─┼─┼─┼─┼─┼─┤\n");
		}

		printf("\t\t\t   └─┴─┴─┴─┴─┴─┴─┴─┘\n");

		// 키보드 입력으로 위치 결정
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		while (true)
		{
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				if (setPos.x < '0' + MAP_WIDTH) setPos.x++;
				else continue;
				break;
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				if (setPos.x > '1') setPos.x--;
				else continue;
				break;
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				if (setPos.y < '@' + MAP_HEIGHT) setPos.y++;
				else continue;
				break;
			}
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				if (setPos.y > 'A') setPos.y--;
				else continue;
				break;
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			{
				setDir = (setDir == RIGHT) ? DOWN : RIGHT;
				break;
			}
			if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			{
				isSelected = true;
				break;
			}
			Sleep(50);
		}

	} while (!isSelected);

}


// 게임 진행 화면 출력
void Render::PrintGameScreen(Position attackPos, std::string attackerName)
{
	int	objIdx;

	system("cls");

	// 현재 턴수 출력
	printf("\n\t\t\t\t    * Turn * \n");
	printf("\t\t\t\t       %02d \n\n", (m_PlayTurn + 1) / 2);

	// 메세지 출력
	for (unsigned int i = 0; i < (76 - m_Message.size()) / 2; ++i)
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
		// Player1의 i번째 라인 맵 출력
		printf(" %c ", 'A' + i);
		printf("│");
		for (int j = 0; j < MAP_WIDTH; ++j)
		{
			objIdx = m_Map1[i][j];
			if (attackPos.x == '1' + j && attackPos.y == 'A' + i && attackerName == m_PlayerName2)
			{
				printf_s("■│");
			}
			else
			{
				printf_s("%c%c│", m_ObjMark[(objIdx + 2) * 2], m_ObjMark[(objIdx + 2) * 2 + 1]);
			}
		}

		printf("\t");

		// Player2의 i번째 라인
		printf(" %c ", 'A' + i);
		printf("│");
		for (int k = 0; k < MAP_WIDTH; ++k)
		{
			objIdx = m_Map2[i][k];
			if (attackPos.x == '1' + k && attackPos.y == 'A' + i && attackerName == m_PlayerName1)
			{
				printf_s("■│");
			}
			else
			{
				printf_s("%c%c│", m_ObjMark[(objIdx + 2) * 2], m_ObjMark[(objIdx + 2) * 2 + 1]);
			}

		}
		printf("\n");

		// 하단라인
		if (i < MAP_HEIGHT - 1){
			printf("   ├─┼─┼─┼─┼─┼─┼─┼─┤");
			printf("\t");
			printf("   ├─┼─┼─┼─┼─┼─┼─┼─┤");
			printf("\n");
		}

	}

	printf("   └─┴─┴─┴─┴─┴─┴─┴─┘");
	printf("\t");
	printf("   └─┴─┴─┴─┴─┴─┴─┴─┘\n");

	// 게이머 이름 출력
	printf("────────┐");
	printf("\t\t\t\t\t     ");
	printf("┌────────\n");


	printf("% 10s\t│", m_PlayerName1.c_str());
	printf("\t\t\t\t\t     ");
	printf("│   % 10s\n", m_PlayerName2.c_str());

	printf("────────┘");
	printf("\t\t\t\t\t     ");
	printf("└────────\n");

}

// notice 화면 출력 오버라이딩
void Render::PrintGameScreen(std::string notice)
{

	int	objIdx;

	system("cls");

	// 현재 턴수 출력
	printf("\n\t\t\t\t    * Turn * \n");
	printf("\t\t\t\t       %02d \n\n", (m_PlayTurn + 1) / 2);

	// 메세지 출력
	for (unsigned int i = 0; i < (76 - m_Message.size()) / 2; ++i)
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
		else if (i < 2 || i > 4)
		{

			// Player1의 i번째 라인 맵 출력
			printf(" %c ", 'A' + i);
			printf("│");
			for (int j = 0; j < MAP_WIDTH; ++j)
			{
				objIdx = m_Map1[i][j];
				if (!m_MapView1 && objIdx > 0) objIdx = 0;
				printf_s("%c%c│", m_ObjMark[(objIdx + 2) * 2], m_ObjMark[(objIdx + 2) * 2 + 1]);
			}

			printf("\t");

			// Player2의 i번째 라인
			printf(" %c ", 'A' + i);
			printf("│");
			for (int k = 0; k < MAP_WIDTH; ++k)
			{
				objIdx = m_Map2[i][k];
				if (!m_MapView2 && objIdx > 0) objIdx = 0;
				printf_s("%c%c│", m_ObjMark[(objIdx + 2) * 2], m_ObjMark[(objIdx + 2) * 2 + 1]);
			}
			printf("\n");

			// 하단라인
			if (i < MAP_HEIGHT - 1){
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


	printf("% 10s\t│", m_PlayerName1.c_str());
	printf("\t\t\t\t\t     ");
	printf("│   % 10s\n", m_PlayerName2.c_str());

	printf("────────┘");
	printf("\t\t\t\t\t     ");
	printf("└────────\n");
}

// 텍스트를 하이라이팅합니다
void Render::HLight(std::string text)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0070);
	std::cout << text;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0007);
}

// 맵 정보 세팅
void Render::SetMap(int mapData1[MAP_HEIGHT][MAP_WIDTH], int mapData2[MAP_HEIGHT][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; ++i)
	{
		for (int j = 0; j < MAP_WIDTH; ++j)
		{
			m_Map1[i][j] = mapData1[i][j];
			m_Map2[i][j] = mapData2[i][j];
		}
	}
}

// 맵 정보 세팅
void Render::SetMap(int mapData1[MAP_HEIGHT][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; ++i)
	{
		for (int j = 0; j < MAP_WIDTH; ++j)
		{
			m_Map1[i][j] = mapData1[i][j];
		}
	}
}

// 플레이어 이름 세팅
void Render::SetPlayerName(std::string playerName1, std::string playerName2)
{
	m_PlayerName1 = playerName1;
	m_PlayerName2 = playerName2;
}