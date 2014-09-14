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
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE,
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
	MISS,
	HIT,
	DESTROY,
	DESTROY_AIRCRAFT,
	DESTROY_BATTLESHIP,
	DESTROY_CRUISER,
	DESTROY_DESTROYER,
};

