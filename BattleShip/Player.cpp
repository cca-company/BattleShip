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
void Player::InitPlayer(Render render, PlayerType type)
{
	// USER일 시 이름 입력받음
	if (type == USER)		m_Name = render.PrintInputNameScreen();
	else if (type == AI)	m_Name = "Computer";

	m_Type = type;
	m_PredictDir = NONE;
	m_PredictPos.x = NULL;
	m_PredictPos.y = NULL;
	m_AttackedPos.clear();
	m_Map.InitMap();
	m_Life = m_ShipList.size();

	SetShip(render);
}

// 배를 배치합니다
void Player::SetShip(Render render)
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
				int	tempMap[MAP_HEIGHT][MAP_WIDTH];
				m_Map.GetField(tempMap);

				render.SetMap(tempMap);
				render.PrintSettingScreen(setPos, setDir, shipHP);
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
Position Player::Attack(Render render)
{
	Position attackPos;
	attackPos.x = '1' + MAP_WIDTH/2;
	attackPos.y = 'A' + MAP_HEIGHT/2;

	while (true)
	{
		// 유저의 경우 위치 Input
		if (m_Type == USER)
		{
			render.PrintGameScreen(attackPos, m_Name);

			while (true)
			{
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
					if (attackPos.x < '0' + MAP_WIDTH){
						attackPos.x++;
						render.PrintGameScreen(attackPos, m_Name);
					}
					else continue;
				}
				if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
					if (attackPos.x > '1'){
						attackPos.x--;
						render.PrintGameScreen(attackPos, m_Name);
					}
					else continue;
				}
				if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				{
					if (attackPos.y < '@' + MAP_HEIGHT){
						attackPos.y++;
						render.PrintGameScreen(attackPos, m_Name);
					}
					else continue;
				}
				if (GetAsyncKeyState(VK_UP) & 0x8000)
				{
					if (attackPos.y > 'A'){
						attackPos.y--;
						render.PrintGameScreen(attackPos, m_Name);
					}
					else continue;
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
			attackPos = AISelectPos();
		}

		// 이미 공격한 위치일 경우
		if (find(m_AttackedPos.begin(), m_AttackedPos.end(), attackPos) != m_AttackedPos.end())
		{
			SetAttackResult(MISS);
			if (m_Type == USER)
			{
				render.SetMessage("This position already entered");
				render.PrintGameScreen(attackPos,m_Name);
			}
			continue;
		}
		break;
	}

	m_AttackedPos.push_back(attackPos);
	return attackPos;
}

// 공격할 위치를 선택합니다
Position Player::AISelectPos()
{
	Position	attackPos;
	bool		isValidPos = false;

	do
	{
		if (m_PredictPos.x == NULL && m_PredictPos.y == NULL)
		{
			attackPos.x = '1' + rand() % MAP_WIDTH;
			attackPos.y = 'A' + rand() % MAP_HEIGHT;
			isValidPos = true;
		}
		else
		{
			Position prevPos = m_AttackedPos.back();

			attackPos.x = prevPos.x;
			attackPos.y = prevPos.y;

			if (m_PredictDir == LEFT) attackPos.x = prevPos.x - 1;
			else if (m_PredictDir == RIGHT)  attackPos.x = prevPos.x + 1;
			else if (m_PredictDir == UP)  attackPos.y = prevPos.y - 1;
			else if (m_PredictDir == DOWN)  attackPos.y = prevPos.y + 1;

			if (attackPos.x < '1' || attackPos.x >= '1' + MAP_WIDTH ||
				attackPos.y < 'A' || attackPos.y >= 'A' + MAP_HEIGHT)
			{
				SetAttackResult(MISS);
				isValidPos = false;
				continue;
			}
			isValidPos = true;
		}
	} while (!isValidPos);
	return attackPos;

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

// attack의 결과를 저장합니다
void Player::SetAttackResult(HitState state)
{
	if (state == HIT)
	{
		if (m_PredictPos.x == NULL && m_PredictPos.y == NULL)
		{
			m_PredictPos = m_AttackedPos.back();
			m_PredictDir = LEFT;
		}
	}
	else if (state == MISS)
	{
		if (m_PredictPos.x != NULL && m_PredictPos.y != NULL)
		{
			m_AttackedPos.push_back(m_PredictPos);
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
				break;
			}
		}
	}
	else // DESTROY
	{
		m_PredictDir = NONE;
		m_PredictPos.x = NULL;
		m_PredictPos.y = NULL;
	}
}

void Player::GetPlayerMap(int map[MAP_HEIGHT][MAP_WIDTH])
{
	m_Map.GetField(map);
}