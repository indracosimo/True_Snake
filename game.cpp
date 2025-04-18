#include "game.h"
#include "snake.h"
#include <iostream>
#include <fstream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <string>
#include <sstream>



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
			centeredText = "Press Enter to play Snake";
			break;

		case GameScreen:: GAMEPLAY:
	
			DrawRectangle(0, 0, screenWidth, screenHeight, VIOLET);
			DrawTilemap();

			if (score == 0) 
			{ 
				LoadLevel(1); 
			}
			else if (score == 1) 
			{ 
				LoadLevel(2); 
			}
			else if (score <= 2)
			{
				LoadLevel(3);
			}

			centeredText = "";			
			DrawRectangle(foodX, foodY, 32, 32, PINK);
			snake.DrawSnake();
			snake.Update();		
			CheckCollisionFood();
			DrawText(TextFormat("Score: %d", score), screenHeight / 8, screenHeight / 16, 40, BLACK);
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
	static int selectedMessageIndex = 0; 

	if (showMessage && !messages.empty())
	{
		if (GetTime() - messageStartTime < 2.0)
		{
			DrawText(messages[selectedMessageIndex].c_str(), eatenFoodX, eatenFoodY, 32, BLACK);
		}
		else
		{
			showMessage = false;
			selectedMessageIndex = rand() % messages.size();
		}
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

	while (!validPosition) 
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
		//prevents spawning in segments
		if (validPosition && snake.addSegment)
		{
			Vector2 newSegmentPosition = Vector2Add(snake.body.front(), snake.direction);
			if (newSegmentPosition.x * tileWidth == foodX && newSegmentPosition.y * tileHeight == foodY)
			{
				validPosition = false;
			}
		}
	}
}

void Game::LoadMessages(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		std::cerr << "Error - Could not open " << filename << std::endl;
		messages.push_back("No file found");
		return;
	}

	std::string line;
	while (getline(file, line)) 
	{
		messages.push_back(line);
	}
	file.close();

	if (messages.empty()) 
	{
		messages.push_back("No messages available");
	}
}