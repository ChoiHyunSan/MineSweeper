#include <iostream>
#include "Game.h"

using namespace std;


int main()
{
	srand((unsigned int)time(nullptr));

	Game game;

	game.play();

	return 0;
}