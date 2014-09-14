#pragma once

struct Position{
	int x;
	int y;
};

enum PlayerType{
	PEOPLE,
	COMPUTER,
	NETWORK,
};

enum HitResult{
	HIT,
	MISS,
	A_DESTROY,
	B_DESTROY,
	C_DESTROY,
	D_DESTROY,
};

enum ShipType{
	SHIP_NONE,
	AIRCRAFT,
	BATTLESHIP,
	CRUISER,
	DESTROYER,
};