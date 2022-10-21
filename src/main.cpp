/**
 * @file main.cpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the overall program system
 */

#include <Windows.h>
#define STB_IMAGE_IMPLEMENTATION
#include "Window.hpp"
#include "Math.hpp"
#include "Renderer.hpp"
#include "Game.hpp"

//==============================================================================
// WinApi main loop

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    Window win("Math Shooter", 1200, 720, hInstance);
    win.SetBackgroundColor(0x242C66);
    Renderer renderer(win);
    Game game;
    game.Init(&renderer);

    while (win.IsRunning())
    {
        renderer.ClearScreen(0x242C66);

        win.HandleMessages();
        renderer.Update(win.GetFt());

        game.Update(&renderer, &win.input, win.GetFt());

        win.ProcessFrame();
    }

    return 0;
}