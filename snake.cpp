#include "snake.h"
#include "game.h"
#include <raymath.h>
#include <raylib.h>

Game game;

void Snake::DrawSnake()
{
	for (int i = 0; i < body.size(); i++)
	{
		float x = body[i].x;
		float y = body[i].y;
		Rectangle segment = Rectangle{ x * snakeCellSize, y * snakeCellSize, (float)snakeCellSize, (float)snakeCellSize };
		DrawRectangleRounded(segment, 0.5, 6, LIME);
	}
}

void Snake::Update() 
{
	//wrap around
	int gridWidth = game.screenWidth / snakeCellSize;
	int gridHeight = game.screenHeight / snakeCellSize;

	if (body[0].x < 0)
	{
		body[0].x = gridWidth - 1;
	}
	else if (body[0].x >= gridWidth)
	{
		body[0].x = 0;
	}

	if (body[0].y < 0)
	{
		body[0].y = gridHeight - 1;
	}
	else if (body[0].y >= gridHeight)
	{
		body[0].y = 0;
	}



	if (IsKeyPressed(KEY_UP) && direction.y != 1 )
		{
			cout << "Up is pressed" << endl;
			nextDirection = { 0, -1 };
		}
	else if (IsKeyPressed(KEY_DOWN) && direction.y != -1)
		{
			cout << "Down is pressed" << endl;
			nextDirection = { 0, 1 };
		}
		else if (IsKeyPressed(KEY_LEFT) && direction.x != 1)
		{
			cout << "Left is pressed" << endl;
			nextDirection = { -1, 0 };
		}
		else if (IsKeyPressed(KEY_RIGHT) && direction.x != -1)
		{
			cout << "Right is pressed" << endl;
			nextDirection = { 1, 0 };
		}

	if (EventTriggered(0.1))
	{
		direction = nextDirection;
		body.push_front(Vector2Add(body[0], direction));
		if (!addSegment)
		{
			body.pop_back();
		}
		addSegment = false;

		if (hasMoved) //prevents spawn dying because of queue 
		{
			CheckCollisionSelf();
		}
		hasMoved = true;
	}

}

//void Snake::Reset()
//{
//	body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };  
//	direction = { 1, 0 }; 
//	nextDirection = { 1, 0 };
//	hasMoved = false;
//}

void Snake::Reset(Vector2 initialPosition)
{
	body = { initialPosition, Vector2{initialPosition.x - 1, initialPosition.y}, Vector2{initialPosition.x - 2, initialPosition.y} };
	direction = { 1, 0 };
	nextDirection = { 1, 0 };
	hasMoved = false;
}

bool Snake::EventTriggered(double interval) 
{
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) 
	{
		lastUpdateTime = currentTime;
		return true;
	}
	else
	{
		return false;
	}
}