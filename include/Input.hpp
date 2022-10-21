/**
 * @file Input.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the Input system's inplementation
 */

#pragma once
#include "Vector.hpp"

#define KEY_DOWN(vkCode) ((GetAsyncKeyState(vkCode) & 0x8000) ? 1 : 0)

//==============================================================================
// Different types of buttons
enum ButtonType
{
    kButtonLeft, 
    kButtonRight,
    kButtonUp,
    kButtonSpace,

    kButtonCount
};

// Button struct
struct Button
{
    bool isDown = false;           // Tells is button is pressed
    bool changed = false;          // Tells is button state is different than the previous one

    /* Sets the state of the button by it's new down value */
    void ProcessState(bool down)
    {
        changed = isDown != down;
        isDown = down;
    }
};

//==============================================================================
// Input struct
struct Input
{
    Vector2f mouse;                 // Mouse position
    Button buttons[kButtonCount];   // All buttons states

    /* Returns true if button is pressed */
    bool Pressed(ButtonType b) { return buttons[b].isDown && buttons[b].changed; }
    /* Returns true if button is released */
    bool Released(ButtonType b) { return !buttons[b].isDown && buttons[b].changed; }
    /* Returns true if button is down */
    bool Down(ButtonType b) { return buttons[b].isDown; }
};
