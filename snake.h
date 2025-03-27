#pragma once
#include <raylib.h>
#include <deque>
#include <iostream>

using namespace std;

class Snake
{
public:
	deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
	Vector2 direction = { 1, 0 };
	Vector2 nextDirection; //used to store the next direction the snake will move in, otherwise you can quickly press keys and bypass movement restrictions
	bool addSegment = false;
	bool hasMoved = false;

	int snakeCellSize = 32;
	int cellCount = 32;
	double lastUpdateTime = 0;

	virtual void DrawSnake();
	virtual void Update();
	virtual void Reset(Vector2 initialPosition);
	bool EventTriggered(double interval);
	bool CheckCollisionSelf();

	bool CheckCollisionWall();
};