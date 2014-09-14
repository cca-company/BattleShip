#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	// Ship ��� ������ �ʱ�ȭ�մϴ�.
	m_ShipList.push_back(new Aircraft());
	m_ShipList.push_back(new Battleship());
	m_ShipList.push_back(new Cruiser());
	m_ShipList.push_back(new Destroyer());
	m_ShipList.push_back(new Destroyer());

}

Player::~Player()
{
	// Ship ��� ������ �޸𸮸� �����մϴ�.
	for (auto& ship : m_ShipList)
	{
		delete ship;
	}
	m_ShipList.clear();
}

// ���� ���۽� �ʱ� ������ �����մϴ�.
void Player::InitPlayer(PlayerType type)
{
	std::string temp;
	m_Type = type;

	if (type == USER)
	{
		std::cout << "�̸��� �Է��ϼ���(�ִ� 10��) : " << std::endl;
		std::cin >> temp;

		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');

		m_Name.assign(temp, 0, 10);
	}
	else if (type == AI) m_Name = "Computer";

	SetShip();
	m_Life = m_ShipList.size();
	m_Map.InitMap();
}

// �踦 ��ġ�մϴ�
void Player::SetShip()
{
	std::string shipName;
	int shipHP;
	int shipIdx = 1;
	Position setPos;
	Direction setDir = NONE;

	for (auto &ship : m_ShipList)
	{
		ship->InitShip();
		shipName = ship->GetName();
		shipHP = ship->GetHP();

		// ��ġ �ʱ� ��ġ ���� ����
		do{
			setPos.x = '1' + rand() % MAP_WIDTH;
			setPos.y = 'A' + rand() % MAP_HEIGHT;
			switch (rand() % 4)
			{
			case 0:
				setDir = LEFT;
				break;
			case 1:
				setDir = RIGHT;
				break;
			case 2:
				setDir = UP;
				break;
			case 3:
				setDir = DOWN;
				break;
			default:
				break;
			}
		} while (!IsValidSetPos(setPos.x, setPos.y, setDir, shipHP));

		// ������ ���� ��ġ�� �ʰ� �� ������ �ݿ�
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

// ���� ������ ���� ��ġ�� ��ȿ���� �˻��մϴ�
bool Player::IsValidSetPos(char setX, char setY, Direction setDir,int shipHP)
{

	switch (setDir)
	{
	case LEFT:
		if (setX - shipHP < '1') return false;
		for (int i = 0; i < shipHP; ++i)
		{
			if (m_Map.CheckPosFull((char)(setX - i), setY)) return false;
		}
		break;
		 
	case RIGHT:
		if (setX + shipHP >= '1' + MAP_WIDTH) return false;
		for (int i = 0; i < shipHP; ++i)
		{
			if (m_Map.CheckPosFull((char)(setX + i), setY)) return false;
		}
		break;

	case UP:
		if (setY - shipHP < 'A') return false;
		for (int i = 0; i < shipHP; ++i)
		{
			if (m_Map.CheckPosFull(setX, (char)(setY - i))) return false;
		}
		break;

	case DOWN:
		if (setY + shipHP >= 'A' + MAP_HEIGHT) return false;
		for (int i = 0; i < shipHP; ++i)
		{
			if (m_Map.CheckPosFull(setX, (char)(setY + i))) return false;
		}
		break;

	default: return false;

	}

	return true;
}

// ���� ��ġ ���� :: �Է� ������ messageBox�� �޼��� ����
bool Player::Attack(Position* attackPos, std::string* messageBox)
{
	std::string temp;
	int			enterKey;

	// ������ ��� ��ġ Input
	if (m_Type == USER)
	{
		enterKey = GetAsyncKeyState(VK_RETURN);


		/*
		std::cin >> temp;

		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');

		if (temp.size() < 2)
		{
			*messageBox = "Invalid position";
			return false;
		}

		attackPos->x = temp.at(1);
		attackPos->y = temp.at(0);

		// �Է� ��ġ ��ȿ�� �˻�
		if (attackPos->y >= 'a' && attackPos->x < 'a' + MAP_HEIGHT) 
			attackPos->y = attackPos->y - 'a' + 'A';

		if (attackPos->x < '1' || attackPos->x >= '1' + MAP_WIDTH ||
			attackPos->y < 'A' || attackPos->y >= 'A' + MAP_HEIGHT)
		{
			*messageBox = "Invalid position";
			return false;
		}
		*/
	}
	// AI�� ��� ���� ����
	else if (m_Type == AI)
	{
		Sleep(1000);
		attackPos->x = '1' + rand() % MAP_WIDTH;
		attackPos->y = 'A' + rand() % MAP_HEIGHT;
	}

	// �̹� ������ ��ġ�� ���
	if (find(m_AttackedPos.begin(), m_AttackedPos.end(), *attackPos) != m_AttackedPos.end())
	{	
		if (m_Type == USER)
		{
			*messageBox = "This position already entered";
		}
		return false;
	}

	m_AttackedPos.push_back(*attackPos);
	return true;
}

// ������ Attack�� Hit�Ǿ����� üũ�մϴ�
HitState Player::HitCheckMyShip(Position attackPos)
{
	HitState result;
	int hitShipIdx;
	hitShipIdx = m_Map.HitMap(attackPos);

	// hit�� ���
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
	int		shipLength = curShip->GetLength();
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