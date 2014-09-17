#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	// Ship 멤버 변수를 초기화합니다.
	m_ShipList.push_back(new Aircraft());
	m_ShipList.push_back(new Battleship());
	m_ShipList.push_back(new Cruiser());
	m_ShipList.push_back(new Destroyer());
	m_ShipList.push_back(new Destroyer());
}

Player::~Player()
{
	// Ship 멤버 변수의 메모리를 해제합니다.
	for (auto& ship : m_ShipList)
	{
		delete ship;
	}
	m_ShipList.clear();
}

// 게임 시작시 초기 정보를 세팅합니다.
void Player::InitPlayer(PlayerType type)
{
	std::string temp;
	m_Type = type;

	if (type == USER)
	{
		system("cls");
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		std::cout << "\n\n\n\n\n\n\n\n\t\t\t  이름을 입력하세요(최대 10자)\n\t\t\t   : ";
		std::cin >> temp;

		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');

		m_Name.assign(temp, 0, 10);
	}
	else if (type == AI) m_Name = "Computer";

	m_PrevState = MISS;
	m_PredictDir = NONE;
	m_PredictPos.x = NULL;
	m_PredictPos.y = NULL;
	m_AttackedPos.clear();
	m_Map.InitMap();
	m_Life = m_ShipList.size();

	SetShip();
}

// 배를 배치합니다
void Player::SetShip()
{
	std::string shipName;
	int shipHP;
	int shipIdx = 1;
	Position setPos;
	Direction setDir = NONE;
	Direction setDirArr[4] = { LEFT, RIGHT, UP, DOWN };

	for (auto &ship : m_ShipList)
	{
		ship->InitShip();
		shipName = ship->GetName();
		shipHP = ship->GetHP();

		// do-while start :: 위치를 지정하고, 유효하지 않은 위치면 반복
		do{
			// 배치 초기 위치 입력받음
			if (m_Type == USER)
			{
				Position cursor;
				char* objMark = "★☆  ⓐⓑⓒⓓⓓ";
				int	objIdx;
				int map[MAP_HEIGHT][MAP_WIDTH];
				int tempHP;
				bool isSelected = false;

				setDir = DOWN;
				m_Map.GetField(map);
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
							objIdx = map[i][j];
							if (cursor.x == '1' + j && cursor.y == 'A' + i)
							{
								printf_s("■│");
								if (tempHP > 1 && setDir == RIGHT) cursor.x++;
								else if(tempHP > 1) cursor.y++;
								--tempHP;
							}
							else
							{
								printf_s("%c%c│", objMark[(objIdx + 2) * 2], objMark[(objIdx + 2) * 2 + 1]);
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
							if (setPos.x < '0'+MAP_WIDTH) setPos.x++;
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
							if (setPos.y < '@'+MAP_HEIGHT) setPos.y++;
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
			// 배치 초기 위치 랜덤 설정
			else if (m_Type == AI)
			{
				setPos.x = '1' + rand() % MAP_WIDTH;
				setPos.y = 'A' + rand() % MAP_HEIGHT;
				setDir = setDirArr[rand() % 4];
			}
		} while (!IsValidSetPos(setPos.x, setPos.y, setDir, shipHP));

		// 결정된 배의 위치를 맵과 배 정보에 반영
		for (int i = 0; i < shipHP; i++)
		{
			ship->SetPos(setPos.x, setPos.y);
			m_Map.SetField(setPos.x, setPos.y, shipIdx);

			if (setDir == LEFT) setPos.x--;
			else if (setDir == RIGHT) setPos.x++;
			else if (setDir == UP) setPos.y--;
			else if (setDir == DOWN) setPos.y++;
		}
		shipIdx++;
	}
}

// 랜덤 결정한 배의 위치가 유효한지 검사합니다
bool Player::IsValidSetPos(char setX, char setY, Direction setDir,int shipHP)
{
	char xPos, yPos;

	// 위치가 유효한지 검사
	if ((setDir == LEFT && setX - shipHP < '1') ||
		(setDir == RIGHT && setX + shipHP > '1' + MAP_WIDTH) ||
		(setDir == UP && setY - shipHP < 'A') ||
		(setDir == DOWN && setY + shipHP > 'A' + MAP_HEIGHT))
	{
		return false;
	}

	// 다른 배가 이미 배치되어있는지 검사
	for (int i = 0; i < shipHP; ++i)
	{
		xPos = setX;
		yPos = setY;
		if (setDir == LEFT) xPos -= i;
		else if (setDir == RIGHT) xPos += i;
		else if (setDir == UP) yPos -= i;
		else if (setDir == DOWN) yPos += i;

		if (m_Map.CheckPosFull(xPos, yPos)) return false;
	}

	return true;
}

// 공격 위치 설정 :: 리턴이 false면 재실행, true면 결정
bool Player::Attack(Position* attackPos, std::string* messageBox)
{
	std::string temp;

	// 유저의 경우 위치 Input
	if (m_Type == USER)
	{
		while (true)
		{
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				if (attackPos->x < '0' + MAP_WIDTH) attackPos->x++;
				else continue;
				return false;
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				if (attackPos->x > '1') attackPos->x--;
				else continue;
				return false;
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				if (attackPos->y < '@' + MAP_HEIGHT) attackPos->y++;
				else continue;
				return false;
			}
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				if (attackPos->y > 'A') attackPos->y--;
				else continue;
				return false;
			}
			if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			{
				break;
			}
			Sleep(50);
		}

	}
	// AI 추적 알고리즘
	else if (m_Type == AI)
	{

		if (m_PredictPos.x == NULL && m_PredictPos.y == NULL )
		{
			if (m_PrevState == HIT)
			{
				m_PredictPos = m_AttackedPos.back();
				m_PredictDir = LEFT;
				return false;
			}
			else
			{
				attackPos->x = '1' + rand() % MAP_WIDTH;
				attackPos->y = 'A' + rand() % MAP_HEIGHT;
			}
		}
		else
		{
			Position prevPos = m_AttackedPos.back();

			if (m_PrevState == MISS)
			{
				prevPos = m_PredictPos;
				switch (m_PredictDir)
				{
				case LEFT:
					m_PredictDir = RIGHT;
					break;
				case RIGHT:
					m_PredictDir = UP;
					break;
				case UP:
					m_PredictDir = DOWN;
					break;
				case DOWN:
					m_PredictDir = NONE;
					m_PredictPos.x = NULL;
					m_PredictPos.y = NULL;
					return false;
					break;
				}
			}
			attackPos->x = prevPos.x;
			attackPos->y = prevPos.y;

			if (m_PredictDir == LEFT) attackPos->x = prevPos.x - 1;
			else if (m_PredictDir == RIGHT)  attackPos->x = prevPos.x + 1;
			else if (m_PredictDir == UP)  attackPos->y = prevPos.y - 1;
			else if (m_PredictDir == DOWN)  attackPos->y = prevPos.y + 1;

			if (attackPos->x < '1' || attackPos->x >= '1' + MAP_WIDTH ||
				attackPos->y < 'A' || attackPos->y >= 'A' + MAP_HEIGHT)
			{
				m_PrevState = MISS;
				return false;
			}
			
			if (m_PrevState != MISS && m_PrevState != HIT)// DESTROY
			{
				m_PredictDir = NONE;
				m_PredictPos.x = NULL;
				m_PredictPos.y = NULL;
				return false;
			}
		}

	}

	// 이미 공격한 위치일 경우
	if (find(m_AttackedPos.begin(), m_AttackedPos.end(), *attackPos) != m_AttackedPos.end())
	{	
		if (m_Type == USER)
		{
			*messageBox = "This position already entered";
		}
		m_PrevState = MISS;
		return false;
	}

	m_AttackedPos.push_back(*attackPos);
	return true;
}

// 상대방의 Attack이 Hit되었는지 체크합니다
HitState Player::HitCheckMyShip(Position attackPos)
{
	HitState result;
	int hitShipIdx;
	hitShipIdx = m_Map.HitMap(attackPos);

	// hit된 경우
	if (hitShipIdx > 0)
	{
		Ship* targetShip = m_ShipList.at(hitShipIdx - 1);
		result = targetShip->GetHitState();

		if (result != HIT) m_Life--;
		return result;
	}
	else
	{
		return MISS;
	}
}

void Player::GetPlayerMap(int map[MAP_HEIGHT][MAP_WIDTH])
{
	m_Map.GetField(map);
}

void Player::PrintMyShip(int shipIdx, bool inverseOp)
{
	Ship*	curShip = m_ShipList.at(shipIdx);
	int		curHP	= curShip->GetHP();

	if (!inverseOp)
	{
		printf("% 10s", curShip->GetName().c_str());
		printf("  ");
	}
	else
	{
		for (int i = 0; i < 5 - curHP; ++i) printf("   ");
	}

	for (int i = 0; i < curShip->GetHP(); ++i)
	{
		curShip->PrintMark();
		printf(" ");
	}

	if (inverseOp)
	{
		printf("% 12s",curShip->GetName().c_str());
	}
	else
	{
		for (int i = 0; i < 5 - curHP; ++i) printf("   ");
	}
}

// 가능성 맵 초기화
void Player::InitPotenMap()
{
	int shipHP;
	Direction dir;

	for (int i = 0; i < MAP_HEIGHT; ++i){
		for (int j = 0; j < MAP_WIDTH; ++j){
			if (m_EnemyMap[i][j] == -1) continue;
			m_EnemyMap[i][j] = 0;
		}
	}

	for (auto& ship : m_EnemyShip)
	{
		shipHP = ship->GetHP();
		for (int i = 0; i < MAP_HEIGHT; ++i){
			for (int j = 0; j < MAP_WIDTH; ++j){

				if (m_EnemyMap[i][j] == -1) continue;

				for (int k = 0; k < shipHP; ++k)
				{
					if (j + k <= MAP_WIDTH - shipHP && m_EnemyMap[i][j + k] == -1) continue;
					if (i + k <= MAP_HEIGHT - shipHP && m_EnemyMap[i + k][j] == -1) continue;
				}

				for (int k = 0; k < shipHP; ++k)
				{
					if (j + k <= MAP_WIDTH - shipHP) m_EnemyMap[i][j + k]++;
					if (i + k <= MAP_HEIGHT - shipHP) m_EnemyMap[i + k][j]++;
				}

			}
		}
	}
}

void Player::SetPotenMap(Position SetPos, int PotenLv)
{
	m_EnemyMap[SetPos.y-'A'][SetPos.x+'1'] = PotenLv;
}

Position Player::GetPotenPos()
{
	std::vector<Position>	maxPotenPosVector;
	Position				maxPotenPos;

	maxPotenPos.x = 0;
	maxPotenPos.y = 0;

	for (int i = 0; i < MAP_HEIGHT; ++i){
		for (int j = 0; j < MAP_WIDTH; ++j){

			if (m_EnemyMap[i][j] < m_EnemyMap[maxPotenPos.y][maxPotenPos.x]) continue;

			if (m_EnemyMap[i][j] > m_EnemyMap[maxPotenPos.y][maxPotenPos.x])
			{
				maxPotenPosVector.clear();
			}

			maxPotenPos.x = j;
			maxPotenPos.y = i;
			maxPotenPosVector.push_back(maxPotenPos);
		
		}
	}

	return maxPotenPosVector.at(rand() % maxPotenPosVector.size());
}

Position Player::HuntNextPos(Position SetPos)
{
	Position maxPotenPos;
	Position aroundPos[4];

	// LEFT
	aroundPos[0].x = SetPos.x - 1;
	aroundPos[0].y = SetPos.y;

	// RIGHT
	aroundPos[1].x = SetPos.x + 1;
	aroundPos[1].y = SetPos.y;

	// UP
	aroundPos[2].x = SetPos.x;
	aroundPos[2].y = SetPos.y - 1;
	
	// DOWN
	aroundPos[3].x = SetPos.x;
	aroundPos[3].y = SetPos.y + 1;

	if (m_PredictDir == NONE)
	{
		for (int i = 0; i < _countof(aroundPos); ++j)
		{
			char x = aroundPos[i].x;
			char y = aroundPos[i].y;
			if ( x >= '1' + MAP_WIDTH ||  x < '1' ||
				y >= 'A' + MAP_HEIGHT ||  y < 'A' ||
				m_EnemyMap[y][x] == 0 )
				continue;

			if (m_EnemyMap[y][x] > )
		}
	}
	else
	{

	}
}