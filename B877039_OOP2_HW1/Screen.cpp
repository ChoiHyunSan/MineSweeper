#include "Screen.h"
#include <string.h>
#include <iostream>
#include <iomanip>
#include "Utils.h"
#pragma warning(disable: 4996)

using namespace std;

void Screen::render(const char* map)
{
	m_map = new char[m_nCols * m_nRows+1];
	
	strcpy(m_map, map);

	Borland::GotoXY(0, 0);
	
	////////////////////////////////////////
	// ¼³Á¤Ã¢

	cout << " ";
	for (int i = 0; i < m_nCols + 2; i++) cout << "¤Ñ";
	cout << endl;

	for (int j = 0; j < 5; j++)
	{
		cout << "|";
		if (j == 2)
		{

			cout << "       MINE GAME       |" << endl;
		}
		else
		{
			for (int i = 0; i < m_nCols * 2 + 2; i++)
			{
				cout << " ";
			}
			cout << " |" << endl;
		}
	}

	////////////////////////////////////////

	cout << " ";
	for (int i = 0; i < m_nCols + 2; i++) cout << "¤Ñ";
	cout << endl;
	
	cout << "|   ";
	for (int i = 0; i < m_nCols; i++)
		cout << i  << " ";
	cout << "|" << endl;

	for (int i = 0; i < m_nCols; ++i)
	{
		cout << "| "<< i << " ";
		for (int j = 0; j < m_nRows; ++j)
		{
			cout << m_map[i * m_nCols + j] << " ";
		}
		cout << "|" << endl;
	}

	cout << " ";
	for (int i = 0; i < m_nCols + 2; i++) cout << "¤Ñ";
	cout << endl;

}

void Screen::setState(GAME_STATE state)
{
}


Screen::Screen(int Rows, int Cols)
	: m_nRows(Rows), m_nCols(Cols) , m_map(nullptr)
{

}

Screen::~Screen()
{
	delete[] m_map;
}
