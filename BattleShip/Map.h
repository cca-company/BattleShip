#pragma once
class Map
{
public:
	/* initialize function */
	Map();
	~Map();
	void			InitMap(); // ���� �ʱ� ������ �����մϴ�

	/* activity function */
	int				HitMap(Position checkPos);  // Ư�� ��ǥ ��Ʈ ����� �����մϴ�
	bool			CheckPosFull(char posX, char posY);  // ���� ��ǥ�� blank ���� �˻� ����� �����մϴ�

	/* state function */
	void			SetField(char posX, char posY, int shipName);  // ���� �ʵ峻 Ư�� ��ǥ�� ���¸� �����մϴ�
	void			GetField(OUT int map[MAP_HEIGHT][MAP_WIDTH]);  // �ʵ带 �����մϴ�

protected:
	/*	< m_Field :: 2���� �迭�� ���� ��ǥ�� ���¸� �����մϴ� >
		0	: �ʱ�ȭ ����(  )
		1~n : n��° �� ���(A~D)
		-2	: ���� ����(��)
		-1	: �ǰ� ����(��)
	*/
	int				m_Field[MAP_HEIGHT][MAP_WIDTH];
};

