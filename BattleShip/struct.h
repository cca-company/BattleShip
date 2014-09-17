#pragma once

// x,y��ǥ ����ü :: �񱳽� x,y�� ��� ������ true�� ����
struct Position{
	char x;
	char y;

	bool operator ==(const Position value)
	{
		if (value.x == x && value.y == y)
			return true;
		else
			return false;
	}
};


// �� ������(����)
enum MapSize{
	MAP_WIDTH = 8,
	MAP_HEIGHT = 8,
};


// �� Ÿ��(����)
enum ShipType{
	AIRCRAFT,
	BATTLESHIP,
	CRUISER,
	DESTROYER,
};


/*  < Direction :: ��ġ �����¿츦 �����մϴ� >
	�ʱ������� � �������� ��ġ���� �����մϴ�
	LEFT,~	: ���� ��,��,��,�츦 ��Ÿ���ϴ�
	NONE	: �ʱⰪ
*/
enum Direction{
	NONE = 0,
	LEFT = 1,
	RIGHT = 2,
	UP = 3,
	DOWN = 4,
};


/*	< PlayerType :: �÷��̾� Ÿ���� �����մϴ� >
	USER	: �Ϲ� �÷��̾�
	AI		: ��ǻ�� ����
	NETWORK : ��Ʈ��ũ ����
*/
enum PlayerType{
	USER,
	AI,
	NETWORK,
};


/*	< HitState :: �� Ÿ�ݽ��� ����� ����մϴ� >
	MISS	: �ƹ��͵� ���� ���� Ÿ���� ���
	HIT		: �踦 Ÿ���Ͽ����� ħ������ ���� ���
	DESTROY : �踦 Ÿ���Ͽ� ħ���� ���(Ship ���� ��ü���� ����)
	DESTROY_~ : Ship��ü���� DESTROY���Ͻ� ���� �ڽ� ��ü���� �������̵�
*/
enum HitState{
	OVER = 0,
	MISS = 1,
	HIT = 2,
	DESTROY = 3,
	DESTROY_AIRCRAFT = 4,
	DESTROY_BATTLESHIP = 5,
	DESTROY_CRUISER = 6,
	DESTROY_DESTROYER = 7,
};

