#include "stdafx.h"


Ship::Ship() : name("ship"), hp(0), direction(0), death(0), shipPosition({ 0, 0 }){}

Ship::Ship(std::string name, int hp) : name(name), hp(hp) {}

Ship::~Ship()
{
}

bool setPosition(Position setPos)
{

}

Position returnPosition(Position setPos)
{

}

bool Ship::HitCheck(Position attackPos)
{
	// attackPos와 자신의 좌표가 맞으면
	return false;
}