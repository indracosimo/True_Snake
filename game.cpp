#include "game.h"
#include "snake.h"
#include <fstream>
#include <raylib.h>
#include <raymath.h>


using namespace std;

Game::Game()
{
	std::cout << "Game object created." << std::endl;
}

Game::~Game() 
{
	std::cout << "Game object deleted." << std::endl;
}

void Game::StartGame()
{
	Vector2 player1StartPos = { 6, 9 };
	Vector2 player2StartPos = { 13, 9 };

	snake.Reset(player1StartPos, false); // Player 1
	snake2.Reset(player2StartPos, true); // Player 2
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
		if (snake.CheckCollisionSelf(false) || snake2.CheckCollisionSelf(true))
		{
			currentScreen = GameScreen::GAMEOVER;
		}
		if (snake.CheckCollisionWall(false) || snake2.CheckCollisionWall(true))
		{
			currentScreen = GameScreen::GAMEOVER;
		}
		CheckCollisionFood(false); // player 1
		CheckCollisionFood(true);	//player2
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
			snake.Reset(Vector2{6, 6}); //top left, right facing direction
			snake2.Reset(Vector2{50, 10});
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
			else if (score >= 2)
			{
				LoadLevel(3);
			}

			centeredText = "";			
			DrawRectangle(foodX, foodY, 32, 32, PINK);
			snake.DrawSnake(false);
			snake.Update(false);
			snake2.DrawSnake(true);
			snake2.Update(true);
			//snakePlayer2.DrawSnake();
			//snakePlayer2.Update();
			//CheckCollisionFood(false);
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

bool Game::CheckCollisionFood(bool isPlayer2)
{
	Snake& currentSnake = isPlayer2 ? snake2 : snake;

	Rectangle snakeHead =
	{
		currentSnake.body[0].x * currentSnake.snakeCellSize,
		currentSnake.body[0].y * currentSnake.snakeCellSize,
		(float)currentSnake.snakeCellSize,
		(float)currentSnake.snakeCellSize
	};

	Rectangle food = { (float)foodX, (float)foodY, (float)currentSnake.snakeCellSize, (float)currentSnake.snakeCellSize };

	if (CheckCollisionRecs(snakeHead, food))
	{
		eatenFoodX = foodX;
		eatenFoodY = foodY;

		showMessage = true;
		messageStartTime = GetTime();
		currentSnake.addSegment = true;
		SpawnFood();
		score += 1;

		cout << "Food eaten by " << (isPlayer2 ? "Player 2" : "Player 1") << endl;
		return true;
	}
	return false;
}

bool Snake::CheckCollisionSelf(bool isPlayer2)
{
	if (!isPlayer2) 
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
	else 
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
		//prevents spawning in head
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