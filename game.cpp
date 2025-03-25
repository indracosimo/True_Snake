#include "game.h"
#include "snake.h"
#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <string>



using namespace std;

Game::Game()
{
	std::cout << "Game object created." << std::endl;
}

Game::~Game() 
{
	std::cout << "Game object deleted." << std::endl;
}

void Game::UpdateMenu(GameScreen& currentScreen, int& frameCounter) 
{
	switch (currentScreen) 
	{
	case GameScreen::LOADING:
		frameCounter++;
		if (frameCounter > 60)
		{
			currentScreen = GameScreen::TITLE;
			frameCounter = 0;
			cout << "Title" << endl;
		}
		break;

	case GameScreen::TITLE:
		if (IsKeyPressed(KEY_ENTER))
		{
			score = 0;
			currentScreen = GameScreen::GAMEPLAY;
			cout << "Gameplay" << endl;
			SpawnFood();
		}
		break;

	case GameScreen::GAMEPLAY:
		if (IsKeyPressed(KEY_P)) 
		{
			currentScreen = GameScreen::PAUSE;
			cout << "Paused" << endl;
		}
		if (IsKeyPressed(KEY_ENTER)) 
		{
			currentScreen = GameScreen::TITLE;
			cout << "Title" << endl;
		}
		if (snake.CheckCollisionSelf()) 
		{
			currentScreen = GameScreen::GAMEOVER;
		}
		if (snake.CheckCollisionWall())
		{
			currentScreen = GameScreen::GAMEOVER;
		}
		break;

		break;
	case GameScreen::PAUSE:
		if (IsKeyPressed(KEY_P))
		{
			currentScreen = GameScreen::GAMEPLAY;
			cout << "Resumed" << endl;
		}
		break;
	case GameScreen::GAMEOVER:
		if(IsKeyPressed(KEY_ENTER))
		{
			score = 0;
			currentScreen = GameScreen::TITLE;
		}
		break;
	}
}

void Game::DrawMenu(GameScreen currentScreen) 
{
	BeginDrawing();
	ClearBackground(RAYWHITE);


	const char* centeredText;
	int fontSize = 40;


	switch (currentScreen)
	{
		case GameScreen::LOADING:
			centeredText = "Loading. . .";
			break;

		case GameScreen:: TITLE:

			DrawRectangle(0, 0, screenWidth, screenHeight, MAGENTA); //background color
			snake.Reset();
			centeredText = "Press Enter to play";
			break;

		case GameScreen:: GAMEPLAY:
	
			DrawRectangle(0, 0, screenWidth, screenHeight, VIOLET);
			DrawTilemap();
			centeredText = "";			
			DrawRectangle(foodX, foodY, 32, 32, PINK);
			snake.DrawSnake();
			snake.Update();		
			CheckCollisionFood();


			DrawText(TextFormat("Hi-Score: %d", score), screenHeight / 8, screenHeight / 16, 40, BLACK);
			break;

		case GameScreen::PAUSE:
			DrawRectangle(0, 0, screenWidth, screenHeight, GRAY);
			centeredText = "Paused";
			break;

		case GameScreen::GAMEOVER:
			DrawRectangle(0, 0, screenWidth, screenHeight, YELLOW);
			centeredText = "GAME OVER";
			break;

		default:
			centeredText = "";
			break;
	}

	////Mm vad gott. . .
	std::string textArray[4] = { "Mm vad gott. . .","Oj vilken goding!","Smarrigt!", "Mums filibabba!"};
	static int selectedMessageIndex = 0; // stores message

	if (showMessage)
	{
		if (GetTime() - messageStartTime < 2.0)
		{
			DrawText(textArray[selectedMessageIndex].c_str(), eatenFoodX, eatenFoodY, 32, BLACK);
		}
		else
		{
			showMessage = false;
		}
	}
	else
	{
		// new message only when showMessage is triggered again
		selectedMessageIndex = rand() % 4;
	}



	int textWidth = MeasureText(centeredText, fontSize); //keeps aspect ratio of text and centers
	int x = (screenWidth - textWidth) / 2;
	int y = screenHeight / 3;

	DrawText(centeredText, x, y, fontSize, BLACK);
		
	EndDrawing();
}

bool Game::CheckCollisionFood()
{
	Rectangle snakeHead = {
		snake.body[0].x * snake.snakeCellSize,
		snake.body[0].y * snake.snakeCellSize,
		(float)snake.snakeCellSize,
		(float)snake.snakeCellSize
	};

	Rectangle food = { (float)foodX, (float)foodY, (float)snake.snakeCellSize, (float)snake.snakeCellSize };

	if (CheckCollisionRecs(snakeHead, food)) 
	{
		eatenFoodX = foodX;
		eatenFoodY = foodY;

		int randomIndex = GetRandomValue(0, 2);

		showMessage = true;
		messageStartTime = GetTime();
		snake.addSegment = true;
		SpawnFood();
		score += 1;

		cout << "Food eaten" << endl;
		return true;
	}
	return false;
}

bool Snake::CheckCollisionSelf()
{
	if (!hasMoved)
		return false;

	for (int i = 1; i < body.size(); i++)
	{
		if (body[0].x == body[i].x && body[0].y == body[i].y)
		{
			cout << "Game Over - Collided with self" << endl;
			return true;
		}
	}
	return false;
}

void Game::SpawnFood()
{
	int borderTileCountX = 1;
	int borderTileCountY = 1;

	int tileWidth = 32;
	int tileHeight = 32;

	int availableWidth = (screenWidth / tileWidth) - borderTileCountX;
	int availableHeight = (screenHeight / tileHeight) - borderTileCountY;

	//check if food is spawning in snake and reposition if true
	bool validPosition = false;

	if (!validPosition) 
	{
		int randomTileX = GetRandomValue(borderTileCountX, availableWidth - 1);
		int randomTileY = GetRandomValue(borderTileCountY, availableHeight - 1);

		foodX = randomTileX * tileWidth;
		foodY = randomTileY * tileHeight;

		validPosition = true;

		for (const auto& segment : snake.body) 
		{
			if (segment.x * tileWidth == foodX && segment.y * tileHeight == foodY)
			{
				validPosition = false;
				break;
			}
		}
	}
}
