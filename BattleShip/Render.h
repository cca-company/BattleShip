#pragma once
#include "stdafx.h"

class Render
{
public:
	Render();
	~Render();

	/* activity function */
	int					PrintStartScreen(); // 게임의 시작화면 출력
	std::string			PrintInputNameScreen(); // 이름 설정화면 출력
	void				PrintSettingScreen(OUT Position& setPos, OUT Direction& setDir, int shipHP); // 설정화면 출력
	void				PrintGameScreen(Position attackPos, std::string attackerName); // 게임 진행 화면 출력
	void				PrintGameScreen(std::string notice); // notice 화면 출력 오버라이딩

	/* state function */
	void				SetMap(int mapData1[MAP_HEIGHT][MAP_WIDTH], int mapData2[MAP_HEIGHT][MAP_WIDTH]); // 맵 정보 세팅
	void				SetMap(int mapData1[MAP_HEIGHT][MAP_WIDTH]); // 맵 정보 세팅
	void				SetPlayerName(std::string playerName1, std::string playerName2); // 플레이어 이름 세팅

	void				SetMessage(std::string message)		{ m_Message = message; } // 출력 메세지 세팅
	void				SetPlayTurn(int playTurn)			{ m_PlayTurn = playTurn; }
	void				SetMapViewOp(bool op1, bool op2)	{ m_MapView1 = op1; m_MapView2 = op2; }
		
protected:
	void				HLight(std::string text); // 텍스트에 하이라이트 처리를 합니다

protected:
	std::string			m_Message;
	int					m_Map1[MAP_HEIGHT][MAP_WIDTH];
	int					m_Map2[MAP_HEIGHT][MAP_WIDTH];
	std::string			m_PlayerName1;
	std::string			m_PlayerName2;
	bool				m_MapView1;
	bool				m_MapView2;
	char*				m_ObjMark = "★☆  ⓐⓑⓒⓓⓓ";
	int					m_PlayTurn;

};

