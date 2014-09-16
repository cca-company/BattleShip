#pragma once
class Map
{
public:
	/* initialize function */
	Map();
	~Map();
	void			InitMap(); // 게임 초기 정보를 세팅합니다

	/* activity function */
	int				HitMap(Position checkPos);  // 특정 좌표 히트 결과를 리턴합니다
	bool			CheckPosFull(char posX, char posY);  // 지정 좌표의 blank 여부 검사 결과를 리턴합니다

	/* state function */
	void			SetField(char posX, char posY, int shipName);  // 가상 필드내 특정 좌표의 상태를 설정합니다
	void			GetField(OUT int map[MAP_HEIGHT][MAP_WIDTH]);  // 필드를 리턴합니다

protected:
	/*	< m_Field :: 2차원 배열에 각각 좌표의 상태를 정의합니다 >
		0	: 초기화 상태(  )
		1~n : n번째 배 요소(A~D)
		-2	: 격추 상태(★)
		-1	: 피격 상태(☆)
	*/
	int				m_Field[MAP_HEIGHT][MAP_WIDTH];
};

