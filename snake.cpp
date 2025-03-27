#include "snake.h"
#include "game.h"
#include <raymath.h>
#include <raylib.h>

Game game;

void Snake::DrawSnake(bool isPlayer2 = false)
{
	for (int i = 0; i < body.size(); i++)
	{
		float x = body[i].x;
		float y = body[i].y;
		Rectangle segment = Rectangle{ x * snakeCellSize, y * snakeCellSize, (float)snakeCellSize, (float)snakeCellSize };
		if (isPlayer2)
		{
			DrawRectangleRounded(segment, 0.5, 6, snakeColor2);
		}
		else 
		{
			DrawRectangleRounded(segment, 0.5, 6, snakeColor);
		}

	}
}

void Snake::Update(bool isPlayer2)
{
	if (!hasMoved)
	{
		direction = nextDirection;
	}
	//wrap around
	int gridWidth = game.screenWidth / snakeCellSize;
	int gridHeight = game.screenHeight / snakeCellSize;

	if (body[0].x < 0) {body[0].x = gridWidth - 1;}
	else if (body[0].x >= gridWidth) {body[0].x = 0;}

	if (body[0].y < 0){body[0].y = gridHeight - 1;}
	else if (body[0].y >= gridHeight) {body[0].y = 0;}


	if (isPlayer2)
	{
		if (IsKeyPressed(KEY_UP) && direction.y != 1)
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
	}
	else 
	{
		if (IsKeyPressed(KEY_W) && direction.y != 1)
		{
			cout << "W is pressed" << endl;
			nextDirection = { 0, -1 };
		}
		else if (IsKeyPressed(KEY_S) && direction.y != -1)
		{
			cout << "S is pressed" << endl;
			nextDirection = { 0, 1 };
		}
		else if (IsKeyPressed(KEY_A) && direction.x != 1)
		{
			cout << "A is pressed" << endl;
			nextDirection = { -1, 0 };
		}
		else if (IsKeyPressed(KEY_D) && direction.x != -1)
		{
			cout << "D is pressed" << endl;
			nextDirection = { 1, 0 };
		}
	}
	

	if (EventTriggered(0.1)) //event triggered 
	{
		direction = nextDirection;
		body.push_front(Vector2Add(body[0], direction));
		if (!addSegment) body.pop_back();
		addSegment = false;

		//prevents spawn dying because of queue 
		if (hasMoved) {CheckCollisionSelf(true);}hasMoved = true;
	}

}

//void Snake::Reset()
//{
//	body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };  
//	direction = { 1, 0 }; 
//	nextDirection = { 1, 0 };
//	hasMoved = false;
//}

void Snake::Reset(Vector2 initialPosition, bool isPlayer2)
{
	if (isPlayer2)
	{
		body = { initialPosition, Vector2{initialPosition.x + 1, initialPosition.y}, Vector2{initialPosition.x + 2, initialPosition.y} };
		direction = { 1, 0 }; // right
		nextDirection = { 1, 0 };
	}
	else
	{
		body = { initialPosition, Vector2{initialPosition.x - 1, initialPosition.y}, Vector2{initialPosition.x - 2, initialPosition.y} };
		direction = { 1, 0 }; //right
		nextDirection = { 1, 0 };
	}
	hasMoved = false;
	//snakeColor = YELLOW;
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