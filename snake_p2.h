#pragma once
#include "snake.h"

class SnakePlayer2 : public Snake
{
public:
    SnakePlayer2();
    void Reset(Vector2 initialPosition) override;
    void Update() override;
};