/**
 * @file Game.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the Game class
 * Those functions defines the game code
 */

#pragma once
#include "Renderer.hpp"
#include "Input.hpp"

class Game
{
private:
public:
    Game();
    ~Game();

    void Init(Renderer *r);
    void Update(Renderer *r, Input *input, float dt);
};
