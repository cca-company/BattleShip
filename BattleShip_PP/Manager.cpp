#include "stdafx.h"


Manager::Manager()
{
}


Manager::~Manager()
{
}

void Manager::createNewGame(std::vector<Player> playerList){
	
	player.getMap(mapSize);

	Ship aircraft("Aircraft", 5);
	Ship battleship("Battleship", 4);
	Ship cruiser("Cruiser", 3);
	Ship destroyer1("Destroyer", 2);
	Ship destroyer2("Destroyer", 2);
	Ship submarin1("Submarin", 1);
	Ship submarin2("Submarin", 1);

	shipData.push_back(aircraft);
	shipData.push_back(battleship);
	shipData.push_back(cruiser);
	shipData.push_back(destroyer1);
	shipData.push_back(destroyer2);
	shipData.push_back(submarin1);
	shipData.push_back(submarin2);

	player.getShip(shipData);
	
}

// std::string setFirstStarter()
// {
// 	int playerNum;
// 	std::string firstStarter;
// 	// playerNum = �÷��̾� ����
// 	// firstStarter = �������� ���� ù �÷��̾�
// 	return firstStarter; // ù �÷��̾� �̸� ��ȯ
// }
// 
// int getPlayerNum()
// {
// 	return 2;
// }
// 
// void setMap()
// {
// 	// Map::drawMap()
// 	// ������ �������� ���� �׸���
// }