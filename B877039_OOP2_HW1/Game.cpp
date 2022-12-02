#include "Game.h"
#include "Screen.h"
#include <vector>
#include "Utils.h"
#include <iostream>
#include <conio.h>
using namespace std;
using std::vector;

Game::Game()
	:
	m_score(0),
	m_nCols(COLS),
	m_nRows(ROWS),
	m_screen(nullptr),
	m_mineNum(MINENUM),
	m_randMineIndex(nullptr),
	m_isGameOver(false)
{
	init();
}

Game::~Game()
{
	if (m_screen != nullptr)
		delete m_screen;

	if (m_mineMap != nullptr)
		delete[] m_mineMap;

	if (m_windowMap != nullptr)
		delete[] m_windowMap;

	if (m_randMineIndex != nullptr)
		delete[] m_randMineIndex;
}

void Game::init()
{

	// Create Screen
	m_screen = new Screen(m_nRows, m_nCols);

	// Init borland
	Borland::Initialize();

	Borland::GotoXY(27, 2);
	cout << "Q : 빈칸";

	Borland::GotoXY(27, 3);
	cout << "X : 지뢰";

	// Init Map
	initMap();

}

void Game::render(GAME_STATE state)
{

	if (state == GAME_STATE::GAME_CLEAR)
	{
		memset(m_windowMap, '~', m_nCols * m_nRows);
		Borland::GotoXY(27, 10);
		cout << "  ㅡㅡㅡㅡㅡㅡㅡ" << endl;
		Borland::GotoXY(27, 11);
		cout << " |  Game Clear  |" << endl;
		Borland::GotoXY(27, 12);
		cout << "  ㅡㅡㅡㅡㅡㅡㅡ" << endl;
	}
	else if (state == GAME_STATE::GAME_OVER)
	{
		memset(m_windowMap, '~', m_nCols * m_nRows);
		Borland::GotoXY(27, 10);
		cout << "  ㅡㅡㅡㅡㅡㅡㅡ" << endl;
		Borland::GotoXY(27, 11);
		cout << " |  Game Over  |" << endl;
		Borland::GotoXY(27, 12);
		cout << "  ㅡㅡㅡㅡㅡㅡㅡ" << endl;
	}

	m_screen->render(m_windowMap);
	
}

GAME_STATE Game::checkGameState()
{
	if (isSearchAllMine()) { return GAME_STATE::GAME_CLEAR; }

	else if (m_isGameOver) { return GAME_STATE::GAME_OVER; }

	else { return GAME_STATE::PLAY; }
}

void Game::update()
{
	render(checkGameState());
	input();
}

// use getch 
void Game::input()
{
	int x(0), y(0);
	char type(0);

	m_input.readEveryFrame();
	
	// 마우스 인풋이 들어온 경우 search()
	Position mousePos = m_input.getMousePosition();
	search((mousePos.x/2 - 2), (mousePos.y - 8));

	// 키패드 숫자 입력값 확인
	for (int i = 0; i < 10; i++)
	{
		if (m_input.getKeyDown(i + 48))
		{
			if (m_keyInputX == -1)
				m_keyInputX = i;
			else if (m_keyInputY == -1)
				m_keyInputY = i;
		}
	}
	// 키보드 인풋이 모두 들어온 경우
	if (m_keyInputX >= 0 && m_keyInputY >= 0)
	{
		search(m_keyInputX, m_keyInputY);
		m_keyInputX = -1;
		m_keyInputY = -1;
	}


}

void Game::play()
{
	while (1)
	{
		update();
	}
}

void Game::setMine()
{
	int count = 0;
	while (count < m_mineNum)
	{
		m_randMineIndex[count] = rand() % (m_nCols * m_nRows - 1);

		for (int i = 0; i < count; i++)
		{
			if (m_randMineIndex[count] == m_randMineIndex[i])
			{
				count--;
				break;
			}
		}	
		count++;
	}
	for (int i = 0; i < m_mineNum; ++i)
	{
		m_mineMap[m_randMineIndex[i]] = MINE;
	}

}

void Game::initMap()
{
	// Init Map
	m_mineMap = new char[m_nCols * m_nRows + 1];
	m_windowMap = new char[m_nCols * m_nRows + 1];

	// Set NULL Char
	m_mineMap[m_nCols * m_nRows] = '\0';
	m_windowMap[m_nCols * m_nRows] = '\0';

	// init Mine Index
	m_randMineIndex = new char[m_mineNum];

	// Set mineMap
	memset(m_mineMap, MAP, m_nCols * m_nRows);

	setMine();

	// Set windowMap
	memset(m_windowMap, MAP, m_nCols * m_nRows);

}

void Game::search(int x, int y)
{
	int selectIndex = 10 * y + x;

	Borland::GotoXY(0, 20);

	if(x >= 0 && y >= 0)
		cout << "input x: " << x << ", input y: " << y << "           ";

	// 지뢰찾기의 크기보다 큰 값이 들어올 경우 무시
	if (x < 0 || y < 0 || x > m_nCols || y > m_nRows) return;

	if (m_mineMap[selectIndex] == MINE) { gameOver(); return; }

	searchMine(x, y, 0);
}

int Game::countMine(int selectIndex)
{
	// 2. 지뢰가 이닌 경우 위치에 따라 연결된 방향에 대한 지뢰 탐색
	vector<int> dirs;

	// 좌표의 위치에 따라 검색할 방향 벡터 컨테이너를 결정
	if (selectIndex % m_nCols == 0)
		dirs = dirLeft;
	else if (selectIndex % m_nCols == m_nCols - 1)
		dirs = dirRight;
	else
		dirs = dirDefault;

	int count = 0;
	for (auto dir : dirs)
	{
		int index = selectIndex + dir;

		if (index >= 0)
		{
			// 지뢰가 설치된 경우 count를 증가
			if (m_mineMap[index] == MINE)
			{
				count++;
			}
		}
	}

	return count;
}

void Game::searchMine(const int x, const int y, int vec)
{
	int selectIndex = 10 * y + x  + vec;

	// 지뢰찾기의 범위 값이 들어올 경우 무시
	if (x < 0 || y < 0 || x > m_nCols || y > m_nRows) return;

	// 2. 지뢰가 이닌 경우 위치에 따라 연결된 방향에 대한 지뢰 탐색

	vector<int> vIndex;
	vector<int> dirs;

	// 좌표의 위치에 따라 검색할 방향 벡터 컨테이너를 결정
	if (x == 0)
		dirs = dirLeft;
	else if (x == m_nCols -1)
		dirs = dirRight;
	else
		dirs = dirDefault;

	int count = 0;
	for (auto dir : dirs)
	{
		int index = selectIndex + dir;

		if (index >= 0)
		{
			// 지뢰가 설치된 경우 count를 증가
			if (m_mineMap[index] == MINE)
			{
				count++;
			}
			// 지뢰가 아닌경우
			else 
			{
				if (m_windowMap[index] == MAP)
				{
					vIndex.push_back(index);
				}
			}
		}
	}

	// 2-1 8방향에 지뢰가 1개 이상인 경우 : 해당 좌표에 지뢰의 개수만큼 표시
	m_windowMap[selectIndex] = static_cast<char>(count + 48);

	// 2-2 8방향에 지뢰가 없는 좌표가 있는 경우 : 해당 좌표에 대한 8방향 지뢰 탐색 함수를 재귀
	for (auto index : vIndex)
	{
		// 지뢰가 아닌 좌표중 인접지뢰가 0인 좌표에 대해서만 재귀함수 호출
		if (countMine(index) == 0)
			searchMine(index % 10, index / 10, 0);
		// 그 외에는 인접지뢰의 개수를 나타낸다.
		else
			m_windowMap[index] = (char)(countMine(index) + 48);
	}

	return;

}

bool Game::isSearchAllMine()
{
	int count = 0;
	for (int i = 0; i < m_nCols * m_nRows; i++)
	{
		if (m_windowMap[i] == MAP)
			count++;
	}
	if (count == m_mineNum)
		return true;

	return false;
}

void Game::markingMine(int x, int y)
{
	// 마킹하려는 좌표가 지뢰가 아니라면 화면의 해당 좌표를 M으로 표시해준다.
}

void Game::gameOver()
{
	cout << "Game Over!" << endl;
}
