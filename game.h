#pragma once

#include <iostream>
#include "raylib.h"
#include "snake.h"

class Game
{
public:

	Snake snake;

	Game();
	~Game();

	enum class GameScreen {LOADING, TITLE, GAMEPLAY, PAUSE, GAMEOVER};

	GameScreen currentScreen = GameScreen::LOADING;

	int score = 0;

	//Food lol
	int foodX = 32;
	int foodY = 32;

	//Silly
	int eatenFoodX = 0;
	int eatenFoodY = 0;
	double messageStartTime = 0.0;
	bool showMessage = false;



	//Screen definitions
	const int screenWidth = 1920;
	const int screenHeight = 1080;

	void UpdateMenu(GameScreen& currentScreen, int& frameCounter);
	void DrawMenu(GameScreen currentScreen);
	void SpawnFood();
	bool CheckCollisionFood();

	void DrawTilemap();
	void LoadLevel(int level);
};