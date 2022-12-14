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
	cout << "Q : 綴蘊";

	Borland::GotoXY(27, 3);
	cout << "X : 雖煆";

	// Init Map
	initMap();

}

void Game::render(GAME_STATE state)
{

	if (state == GAME_STATE::GAME_CLEAR)
	{
		memset(m_windowMap, '~', m_nCols * m_nRows);
		Borland::GotoXY(27, 10);
		cout << "  天天天天天天天" << endl;
		Borland::GotoXY(27, 11);
		cout << " |  Game Clear  |" << endl;
		Borland::GotoXY(27, 12);
		cout << "  天天天天天天天" << endl;
	}
	else if (state == GAME_STATE::GAME_OVER)
	{
		memset(m_windowMap, '~', m_nCols * m_nRows);
		Borland::GotoXY(27, 10);
		cout << "  天天天天天天天" << endl;
		Borland::GotoXY(27, 11);
		cout << " |  Game Over  |" << endl;
		Borland::GotoXY(27, 12);
		cout << "  天天天天天天天" << endl;
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
	
	// 葆辦蝶 檣Е檜 菟橫螞 唳辦 search()
	Position mousePos = m_input.getMousePosition();
	search((mousePos.x/2 - 2), (mousePos.y - 8));

	// 酈ぬ萄 璋濠 殮溘高 ?挫?
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
	// 酈爾萄 檣Е檜 賅舒 菟橫螞 唳辦
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

	// 雖煆瓊晦曖 觼晦爾棻 贖 高檜 菟橫螢 唳辦 鼠衛
	if (x < 0 || y < 0 || x > m_nCols || y > m_nRows) return;

	if (m_mineMap[selectIndex] == MINE) { gameOver(); return; }

	searchMine(x, y, 0);
}

int Game::countMine(int selectIndex)
{
	// 2. 雖煆陛 檜棋 唳辦 嬪纂縑 評塭 翱唸脹 寞щ縑 渠и 雖煆 鬚儀
	vector<int> dirs;

	// 謝ル曖 嬪纂縑 評塭 匐儀й 寞щ 漱攪 鐘纔檜傘蒂 唸薑
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
			// 雖煆陛 撲纂脹 唳辦 count蒂 隸陛
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

	// 雖煆瓊晦曖 彰嬪 高檜 菟橫螢 唳辦 鼠衛
	if (x < 0 || y < 0 || x > m_nCols || y > m_nRows) return;

	// 2. 雖煆陛 檜棋 唳辦 嬪纂縑 評塭 翱唸脹 寞щ縑 渠и 雖煆 鬚儀

	vector<int> vIndex;
	vector<int> dirs;

	// 謝ル曖 嬪纂縑 評塭 匐儀й 寞щ 漱攪 鐘纔檜傘蒂 唸薑
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
			// 雖煆陛 撲纂脹 唳辦 count蒂 隸陛
			if (m_mineMap[index] == MINE)
			{
				count++;
			}
			// 雖煆陛 嬴棋唳辦
			else 
			{
				if (m_windowMap[index] == MAP)
				{
					vIndex.push_back(index);
				}
			}
		}
	}

	// 2-1 8寞щ縑 雖煆陛 1偃 檜鼻檣 唳辦 : п渡 謝ル縑 雖煆曖 偃熱虜躑 ル衛
	m_windowMap[selectIndex] = static_cast<char>(count + 48);

	// 2-2 8寞щ縑 雖煆陛 橈朝 謝ル陛 氈朝 唳辦 : п渡 謝ル縑 渠и 8寞щ 雖煆 鬚儀 л熱蒂 營敝
	for (auto index : vIndex)
	{
		// 雖煆陛 嬴棋 謝ル醞 檣蕾雖煆陛 0檣 謝ル縑 渠п憮虜 營敝л熱 ??轎
		if (countMine(index) == 0)
			searchMine(index % 10, index / 10, 0);
		// 斜 諼縑朝 檣蕾雖煆曖 偃熱蒂 釭顫魚棻.
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
	// 葆韁ж溥朝 謝ル陛 雖煆陛 嬴棲塭賊 ?飛橉? п渡 謝ル蒂 M戲煎 ル衛п遽棻.
}

void Game::gameOver()
{
	cout << "Game Over!" << endl;
}
