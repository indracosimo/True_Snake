//#include "snake_p2.h"
//#include "game.h"
//#include <raymath.h>
//#include <raylib.h>
//
//extern Game game;
//
//SnakePlayer2::SnakePlayer2() : Snake()
//{
//    Reset({ 10, 10 }); // Initial position for player 2
//}
//
//void SnakePlayer2::Reset(Vector2 initialPosition)
//{
//    body = { initialPosition, Vector2{initialPosition.x - 1, initialPosition.y}, Vector2{initialPosition.x - 2, initialPosition.y} };
//    direction = { 1, 0 };
//    nextDirection = { 1, 0 };
//    hasMoved = false;
//}
//
//void SnakePlayer2::Update()
//{
//
//    // Handle input for player 2
//    if (IsKeyPressed(KEY_W) && direction.y != 1)
//    {
//        nextDirection = { 0, -1 };
//    }
//    else if (IsKeyPressed(KEY_S) && direction.y != -1)
//    {
//        nextDirection = { 0, 1 };
//    }
//    else if (IsKeyPressed(KEY_A) && direction.x != 1)
//    {
//        nextDirection = { -1, 0 };
//    }
//    else if (IsKeyPressed(KEY_D) && direction.x != -1)
//    {
//        nextDirection = { 1, 0 };
//    }
//
//    // Wrap around logic
//    int gridWidth = game.screenWidth / snakeCellSize;
//    int gridHeight = game.screenHeight / snakeCellSize;
//
//    if (body[0].x < 0)
//    {
//        body[0].x = gridWidth - 1;
//    }
//    else if (body[0].x >= gridWidth)
//    {
//        body[0].x = 0;
//    }
//
//    if (body[0].y < 0)
//    {
//        body[0].y = gridHeight - 1;
//    }
//    else if (body[0].y >= gridHeight)
//    {
//        body[0].y = 0;
//    }
//
//    if (EventTriggered(0.1))
//    {
//        direction = nextDirection;
//        body.push_front(Vector2Add(body[0], direction));
//        if (!addSegment)
//        {
//            body.pop_back();
//        }
//        addSegment = false;
//
//        if (hasMoved) // prevents spawn dying because of queue
//        {
//            CheckCollisionSelf();
//        }
//        hasMoved = true;
//    }
//}