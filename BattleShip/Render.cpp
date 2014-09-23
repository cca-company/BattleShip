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

// ������ ����ȭ�� ���
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

// �̸� ����ȭ�� ���
std::string Render::PrintInputNameScreen()
{
	std::string temp;
	std::string result;

	system("cls");
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	std::cout << "\n\n\n\n\n\n\n\n\t\t\t  �̸��� �Է��ϼ���(�ִ� 10��)\n\t\t\t   : ";
	std::cin >> temp;

	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');

	result.assign(temp, 0, 10);

	return result;
}

// �� ��ġ ����ȭ�� ���
void Render::PrintSettingScreen(OUT Position& setPos, OUT Direction& setDir, int shipHP)
{
	Position		cursor;
	int				objIdx;
	int				tempHP;
	bool			isSelected = false;

	setDir = DOWN;
	setPos.x = '1' + MAP_WIDTH / 2 - 1;
	setPos.y = 'A' + MAP_HEIGHT / 2 - 1;

	// do-while :: �� ���
	do{
		cursor = setPos;
		tempHP = shipHP;
		system("cls");

		printf("\n\n\n\t\t\t     �踦 ��ġ�� ��ġ�� �����ϼ���\n\n");

		printf("\t\t\t   ");
		for (int i = 0; i < MAP_WIDTH; ++i) printf("   %c", '1' + i);

		printf("\n\t\t\t   ����������������������������������\n");

		for (int i = 0; i < MAP_HEIGHT; ++i)
		{
			// Player1�� i��° ���� �� ���
			printf("\t\t\t %c ", 'A' + i);
			printf("��");
			for (int j = 0; j < MAP_WIDTH; ++j)
			{
				objIdx = m_Map1[i][j];
				if (cursor.x == '1' + j && cursor.y == 'A' + i)
				{
					printf_s("�ᦢ");
					if (tempHP > 1 && setDir == RIGHT) cursor.x++;
					else if (tempHP > 1) cursor.y++;
					--tempHP;
				}
				else
				{
					printf_s("%c%c��", m_ObjMark[(objIdx + 2) * 2], m_ObjMark[(objIdx + 2) * 2 + 1]);
				}
			}

			printf("\n");

			// �ϴܶ���
			if (i < MAP_HEIGHT - 1) printf("\t\t\t   ����������������������������������\n");
		}

		printf("\t\t\t   ����������������������������������\n");

		// Ű���� �Է����� ��ġ ����
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


// ���� ���� ȭ�� ���
void Render::PrintGameScreen(Position attackPos, std::string attackerName)
{
	int	objIdx;

	system("cls");

	// ���� �ϼ� ���
	printf("\n\t\t\t\t    * Turn * \n");
	printf("\t\t\t\t       %02d \n\n", (m_PlayTurn + 1) / 2);

	// �޼��� ���
	for (unsigned int i = 0; i < (76 - m_Message.size()) / 2; ++i)
	{
		printf(" ");
	}
	printf("[ %s ]\n\n", m_Message.c_str());

	// �� ���
	printf("   ");
	for (int i = 0; i < MAP_WIDTH; ++i) printf("   %c", '1' + i);
	printf("\t   ");
	for (int i = 0; i < MAP_WIDTH; ++i) printf("   %c", '1' + i);
	printf("\n");

	printf("   ����������������������������������");
	printf("\t");
	printf("   ����������������������������������\n");


	for (int i = 0; i < MAP_HEIGHT; ++i)
	{
		// Player1�� i��° ���� �� ���
		printf(" %c ", 'A' + i);
		printf("��");
		for (int j = 0; j < MAP_WIDTH; ++j)
		{
			objIdx = m_Map1[i][j];
			if (attackPos.x == '1' + j && attackPos.y == 'A' + i && attackerName == m_PlayerName2)
			{
				printf_s("�ᦢ");
			}
			else
			{
				printf_s("%c%c��", m_ObjMark[(objIdx + 2) * 2], m_ObjMark[(objIdx + 2) * 2 + 1]);
			}
		}

		printf("\t");

		// Player2�� i��° ����
		printf(" %c ", 'A' + i);
		printf("��");
		for (int k = 0; k < MAP_WIDTH; ++k)
		{
			objIdx = m_Map2[i][k];
			if (attackPos.x == '1' + k && attackPos.y == 'A' + i && attackerName == m_PlayerName1)
			{
				printf_s("�ᦢ");
			}
			else
			{
				printf_s("%c%c��", m_ObjMark[(objIdx + 2) * 2], m_ObjMark[(objIdx + 2) * 2 + 1]);
			}

		}
		printf("\n");

		// �ϴܶ���
		if (i < MAP_HEIGHT - 1){
			printf("   ����������������������������������");
			printf("\t");
			printf("   ����������������������������������");
			printf("\n");
		}

	}

	printf("   ����������������������������������");
	printf("\t");
	printf("   ����������������������������������\n");

	// ���̸� �̸� ���
	printf("������������������");
	printf("\t\t\t\t\t     ");
	printf("������������������\n");


	printf("% 10s\t��", m_PlayerName1.c_str());
	printf("\t\t\t\t\t     ");
	printf("��   % 10s\n", m_PlayerName2.c_str());

	printf("������������������");
	printf("\t\t\t\t\t     ");
	printf("������������������\n");

}

// notice ȭ�� ��� �������̵�
void Render::PrintGameScreen(std::string notice)
{

	int	objIdx;

	system("cls");

	// ���� �ϼ� ���
	printf("\n\t\t\t\t    * Turn * \n");
	printf("\t\t\t\t       %02d \n\n", (m_PlayTurn + 1) / 2);

	// �޼��� ���
	for (unsigned int i = 0; i < (76 - m_Message.size()) / 2; ++i)
	{
		printf(" ");
	}
	printf("[ %s ]\n\n", m_Message.c_str());

	// �� ���
	printf("   ");
	for (int i = 0; i < MAP_WIDTH; ++i) printf("   %c", '1' + i);
	printf("\t   ");
	for (int i = 0; i < MAP_WIDTH; ++i) printf("   %c", '1' + i);
	printf("\n");

	printf("   ����������������������������������");
	printf("\t");
	printf("   ����������������������������������\n");

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
			printf("   ����������������������������������");
			printf("\t");
			printf("   ����������������������������������");
			printf("\n");
		}
		else if (i < 2 || i > 4)
		{

			// Player1�� i��° ���� �� ���
			printf(" %c ", 'A' + i);
			printf("��");
			for (int j = 0; j < MAP_WIDTH; ++j)
			{
				objIdx = m_Map1[i][j];
				if (!m_MapView1 && objIdx > 0) objIdx = 0;
				printf_s("%c%c��", m_ObjMark[(objIdx + 2) * 2], m_ObjMark[(objIdx + 2) * 2 + 1]);
			}

			printf("\t");

			// Player2�� i��° ����
			printf(" %c ", 'A' + i);
			printf("��");
			for (int k = 0; k < MAP_WIDTH; ++k)
			{
				objIdx = m_Map2[i][k];
				if (!m_MapView2 && objIdx > 0) objIdx = 0;
				printf_s("%c%c��", m_ObjMark[(objIdx + 2) * 2], m_ObjMark[(objIdx + 2) * 2 + 1]);
			}
			printf("\n");

			// �ϴܶ���
			if (i < MAP_HEIGHT - 1){
				printf("   ����������������������������������");
				printf("\t");
				printf("   ����������������������������������");
				printf("\n");
			}

		}

	}

	printf("   ����������������������������������");
	printf("\t");
	printf("   ����������������������������������\n");

	// ���̸� �̸� ���
	printf("������������������");
	printf("\t\t\t\t\t     ");
	printf("������������������\n");


	printf("% 10s\t��", m_PlayerName1.c_str());
	printf("\t\t\t\t\t     ");
	printf("��   % 10s\n", m_PlayerName2.c_str());

	printf("������������������");
	printf("\t\t\t\t\t     ");
	printf("������������������\n");
}

// �ؽ�Ʈ�� ���̶������մϴ�
void Render::HLight(std::string text)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0070);
	std::cout << text;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0007);
}

// �� ���� ����
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

// �� ���� ����
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

// �÷��̾� �̸� ����
void Render::SetPlayerName(std::string playerName1, std::string playerName2)
{
	m_PlayerName1 = playerName1;
	m_PlayerName2 = playerName2;
}