#pragma once
#pragma warning(disable : 4996)
#include <vector>
using std::vector;
#include "input.h"
class Screen;

#define COLS 10
#define ROWS 10
#define MINE 'X'
#define MAP 'Q'
#define MINENUM 15

// 할거
// 1. 재귀적인 지뢰없는 맵 찾기
// 2. 입력값 받기
// 3. 게임 외관꾸미기
// 4. 게임오버 상태 만들기
// 5. 승리 조건 추가, 클리어 화면 추가

enum class GAME_STATE
{
	TITLE = 0,
	PLAY,
	GAME_OVER,
	GAME_CLEAR
};

class Game
{
private:
	Screen* m_screen; // 
	Input	m_input;

	// Screen Scale
	int		m_nCols;
	int		m_nRows;

	// Map
	char*	m_mineMap;
	char*	m_windowMap;
	
	// etc
	int		m_mineNum;
	char*	m_randMineIndex;

	int		m_keyInputX = -1;
	int		m_keyInputY = -1;

	bool    m_isGameOver;

	int		m_score;	  // 게임 스코어 : 지뢰를 밟지 않고 넓힌 땅의 크기

private:
	vector<int> dirDefault = { (-m_nCols - 1) , (-m_nCols), (1 - m_nCols), -1, 1, (m_nCols - 1),m_nCols, (m_nCols + 1) };
	vector<int> dirRight = { (-m_nCols - 1) , (-m_nCols), -1, (m_nCols - 1), m_nCols };
	vector<int> dirLeft = { (-m_nCols), (1 - m_nCols),  1, m_nCols, (m_nCols + 1) };

public:


private:
	void init();
	void render(GAME_STATE state);
	void update();
	void input();

private:
	void setMine();
	void initMap();

	int countMine(int selectIndex);

	void markingMine(int x, int y);

	void search(int x, int y);
	void searchMine(int x, int y, int vec);

	bool isSearchAllMine();

	GAME_STATE checkGameState();

private:
	void gameOver();

public:
	void play();

	Game();
	virtual ~Game();
};

