#pragma once
#include "Game.h"

class Screen
{
private:
	// Screen Scale
	int			m_nCols;
	int			m_nRows;

	char*		m_map;

	GAME_STATE	m_state;

public:
	void render(const char* map);
	void setState(GAME_STATE state);

private:


public:
	Screen(int Rows, int Cols);
	~Screen();
};

