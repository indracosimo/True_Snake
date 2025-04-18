#include "game.h"
#include "snake.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <string>

const int mapWidth = 64;
const int mapHeight = 36;
const int tileSize = 60;

int levelMap[mapHeight][mapWidth];


void Game::DrawTilemap()
{
	int tileSize = 32;

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			if (levelMap[y][x] == 1)
			{
				DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, DARKPURPLE);
			}
		}
	}
}


bool Snake::CheckCollisionWall()
{
	int tileSize = 32;
	int gridX = body[0].x;
	int gridY = body[0].y;

	if (levelMap[gridY][gridX] == 1)
	{
		std::cout << "Game Over - Collided with wall" << std::endl;
		return true;
	}

	return false;
}

int level1[mapHeight][mapWidth] = {  };
int level2[mapHeight][mapWidth] = {  };

void Game::LoadLevel(int level)
{
	std::ifstream file;
	std::string filename;
	 
	if (level == 1) 
	{
		filename = "level1.txt";
	}
	else if (level == 2)
	{
		filename = "level2.txt";
	}
	else if (level == 3)
	{
		filename = "level3.txt";
	}
	else 
	{
		std::cerr << "Invalid level" << endl;
		return;
	}

	file.open(filename);
	if (!file.is_open()) 
	{
		std::cerr << "Error opening file: " << filename << endl;
		return;
	}

	for (int y = 0; y < mapHeight; y++) {
		std::getline(file, filename);
		std::stringstream ss(filename);
		for (int x = 0; x < mapWidth; x++) {
			ss >> levelMap[y][x]; 
		}
	}

	file.close();
}
