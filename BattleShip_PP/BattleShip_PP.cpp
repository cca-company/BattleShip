// BattleShip_pp.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

std::vector<std::string> checkUnits;

int _tmain(int argc, _TCHAR* argv[])
{

	Player player; // �Ϲ� ����
	/*
	Player * pPlayer; = new Player(); // ���� ����
	delete(pPlayer);
	*/

	//UNREFERENCED_PARAMETER(argc);
	argc;
	argv;

	Manager gameManager;
	std::vector<Player> playerList;
	Player playerOwn;
	playerList.push_back(playerOwn);  

	gameManager.createNewGame(playerList);

	
	
	
	std::string input_str;
	std::string guess_request = "��ǥ �Է�: ";
	std::string respone[] = { "HIT", "MISS", "DESTROY" };

	srand(time(NULL));

	while (!(units.empty()))
	{
		system("cls");
		for (int i = 0; i < units.size(); i++)
		{
			std::cout << units[i] << "   ";
		}
		std::cout << std::endl;

		std::cout << guess_request;
		std::cin >> input_str;

		if (input_str == "quit") break;
		int random = rand() % _countof(respone);
		int randPick = rand() % units.size();


		switch (random)
		{
		case 0:
			std::cout << respone[random] << std::endl;
			break;
		case 1:
			std::cout << respone[random] << std::endl;
			break; 
		case 2:
			std::cout << units[randPick] << " is DESTROYED!" << std::endl;
			units.erase(units.begin() + randPick);
			break;
		}
		std::cout << "\nNext Turn!" << std::endl;
		getchar();
		getchar();
	}
	std::cout << "You win!" << std::endl;
	getchar();

	return 0;
}

