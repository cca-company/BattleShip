#pragma once

class Manager
{
public:
	Manager();
	~Manager();
	void createNewGame(std::vector<Player> playerList);
	void allocateShip(std::vector<Player> playerList);
	bool shipPosIsValid(Map userMap, std::vector<Ship> shipList);
	void setFirstStarter(std::vector<Player> playerList);
	void startGame(std::vector<Player> playerList);
	void decision(Map userMap, std::vector<Ship> shipList);

private:
	Position mapSize;

};

