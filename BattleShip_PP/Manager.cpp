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
// 	// playerNum = 플레이어 숫자
// 	// firstStarter = 랜덤으로 정한 첫 플레이어
// 	return firstStarter; // 첫 플레이어 이름 반환
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
// 	// 정해진 사이즈의 맵을 그린다
// }