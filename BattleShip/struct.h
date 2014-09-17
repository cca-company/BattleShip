#pragma once

// x,y좌표 구조체 :: 비교시 x,y가 모두 같으면 true를 리턴
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


// 맵 사이즈(고정)
enum MapSize{
	MAP_WIDTH = 8,
	MAP_HEIGHT = 8,
};


// 배 타입(고정)
enum ShipType{
	AIRCRAFT,
	BATTLESHIP,
	CRUISER,
	DESTROYER,
};


/*  < Direction :: 배치 상하좌우를 설정합니다 >
	초기점에서 어떤 방향으로 배치할지 결정합니다
	LEFT,~	: 각각 상,하,좌,우를 나타냅니다
	NONE	: 초기값
*/
enum Direction{
	NONE = 0,
	LEFT = 1,
	RIGHT = 2,
	UP = 3,
	DOWN = 4,
};


/*	< PlayerType :: 플레이어 타입을 설정합니다 >
	USER	: 일반 플레이어
	AI		: 컴퓨터 대전
	NETWORK : 네트워크 대전
*/
enum PlayerType{
	USER,
	AI,
	NETWORK,
};


/*	< HitState :: 배 타격시의 결과를 출력합니다 >
	MISS	: 아무것도 없는 곳을 타격한 경우
	HIT		: 배를 타격하였으나 침몰하지 않은 경우
	DESTROY : 배를 타격하여 침몰한 경우(Ship 상위 객체에서 리턴)
	DESTROY_~ : Ship객체에서 DESTROY리턴시 각각 자식 객체에서 오버라이딩
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

