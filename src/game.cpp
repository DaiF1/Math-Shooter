/**
 * @file game.cpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the game code along with Game class's implementation
 */

#include "Game.hpp"
#include "Math.hpp"
#include "Bullet.hpp"
#include "Platform.hpp"
#include "Equation.hpp"
#include <vector>

Game::Game() {}
Game::~Game() {}

//==============================================================================
// Game variables

Vector2i pos = {100,100};                   // Player position
Vector2i hSize = {10,20};                   // Player half size
int dir = 1;                                // Player direction

int jumpHeight = 20;                        // Player jump height
bool jumped = false;                        // If player is jumping
bool onGround = true;                       // If player is on ground
int gravity = 0;                            // Gravity power

int life = 100;                             // Player life

std::vector<Bullet *> bullets;              // Player bullets

std::vector<Platform *> platforms = {       // Game platforms
    new Platform({300, 200}, {100, 10}),
    new Platform({880, 270}, {80, 10}),
    new Platform({600, 350}, {110, 10}),
    new Platform({400, 470}, {60, 10})
};

int currentPlat;                            // Player's current platform

std::vector<Equation *> equations;          // Game enemies
const char *equPaths[4] = {
    "res\\equ1.png",
    "res\\equ2.png",
    "res\\equ3.png",
    "res\\equ4.png"
};

//==============================================================================
// Game functions

void Game::Init(Renderer *r)
{
    for (int i = 0; i < 4; i++)
    {
        Equation *equ = new Equation({r->GetBufferWidth()+150*i, 
            (rand() % 3)*200 + 100}, 5, equPaths[i]);
        r->objectsToDraw.push_back(equ->GetSprite());
        equations.push_back(equ); 
    }
}

void Game::Update(Renderer *r, Input *input, float dt)
{
    //////////// Player Physics ///////////

    // Movement
    int x = 0;
    if (input->Down(kButtonLeft)) 
    {
        x -= 5;
        dir = -1;
    }
    if (input->Down(kButtonRight))
    {
        x += 5;
        dir = 1;
    }
    pos.x += x;

    // Jump
    if (input->Pressed(kButtonUp) && !jumped && onGround) 
    {
        jumped = true;
        onGround = false;
    }
    
    if (jumped)
    {
        pos.y += jumpHeight;
        if (onGround)
            jumped = false;
    }

    // Gravity
    if (!onGround) gravity++;

    if (pos.y-gravity <= 100)
    {
        onGround = true;
        jumped = false;
        pos.y = 100;
        gravity = 0;
    }
    pos.y -= gravity;

    // Platforms collisions
    for (int i=0; i < (int)platforms.size(); i++)
    {
        Platform *p = platforms.at(i);

        bool collision = p->Collide(pos, hSize);
        if (!collision && currentPlat == i && pos.y > 100)
        {
            onGround = false;
            currentPlat = -1;
        }
        else if (collision && (!jumped || jumpHeight-gravity < 0) && currentPlat == -1)
        {
            pos.y = p->Ground()+hSize.y;
            onGround = true;
            jumped = false;
            gravity = 0;
            currentPlat = i;
        }
    }
    /////////// Game Management ///////////

    // Life bar
    r->DrawRect({220, r->GetBufferHeight()-20}, {200, 10}, 0x282C34);
    r->DrawRect({20+life*2, r->GetBufferHeight()-20}, {life*2, 10}, 0xE6C440);

    ///////////// Game Assets /////////////

    // Bullets
    if (input->Pressed(kButtonSpace))
        bullets.push_back(new Bullet(pos, dir));

    for (int i=0; i < (int)bullets.size(); i++)
    {
        Bullet *b = bullets[i];
        b->Update(r);
        for (Equation *equ : equations)
        {
            if (b->Hit(equ->GetPos(), equ->GetSize()*1.5f))
            {
                equ->GetHit();
                bullets.erase(bullets.begin()+i);
                i--;
                continue;
            }
        }
        b->Draw(r);
        if (b->NotOnScreen())
            bullets.erase(bullets.begin()+i);
    }
    
    // Equations
    for (Equation *equ : equations)
    {
        equ->Update();
        if (equ->OutOfBound())
        {
            equ->Restart();
            life -= 10;
        }
    }

    r->DrawRect({600, 40}, {600, 40}, 0x4049E6);
    r->DrawRect(pos, hSize, 0xE6C440);

    for (int i=0; i < (int)platforms.size(); i++)
    {
        Platform *p = platforms.at(i);
        p->Draw(r);
    }
}