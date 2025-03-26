#include"game.h"
#include"snake.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include<raylib.h>
#include<iostream>

using namespace std;

int main() 
{
	Game* game = new Game();

	game->LoadMessages("foodEaten.txt");

	game->LoadLevel(1);

	cout << "Loading..." << endl;
	InitWindow(game->screenWidth, game->screenHeight, "Game");

	game->currentScreen = Game::GameScreen::LOADING;
	int frameCounter = 0;

	SetTargetFPS(60);


	
	while (!WindowShouldClose())
	{
		game->UpdateMenu(game->currentScreen, frameCounter);
		game->DrawMenu(game->currentScreen);

	}

	delete game;
	CloseWindow();

	return 0;
}

