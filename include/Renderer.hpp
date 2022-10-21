/**
 * @file Renderer.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the Renderer class. 
 * It is used to render objects on screen
 */

#pragma once

#include <Windows.h>
#include <vector>

#include "Window.hpp"
#include "Vector.hpp"
#include "FileReader.hpp"

class Sprite;
class AnimatedSprite;
struct Rect;

struct Text;
struct Font;

#define SWAP(a, b) do { auto temp = a; a = b; b = temp; } while(0)

//==============================================================================
// Renderer class 
class Renderer
{
private:
    WinBuffer *_buffer;                     // Pointer to pixel buffer
    Vector2i _desiredCam = {0,0};           // Desired camera position. Used for smooth transitions
    Vector2i _cam = {0,0};                  // Current camera position
    float _transitionTime = 0;              // Time elapsed during camera transition to desired position 
    
    /* Draws text on screen */
    void PrintText(Text text);
    /* Draws a letter on screen */
    void DrawLetter(Font *font, int index, Vector2i pos, Vector2i hSize);
    /* Draws a rectangle with rounded borders */
    void DrawRoundedRect(Vector2i pos, Vector2i hSize, int radius, unsigned int color);

public:
    /**
     * Constructor
     * Takes a window instance
     */
    Renderer(Window win);
    ~Renderer();
    
    /**
     * Draws a rectangle on screen
     * Takes it's position, half size and color
     */
    void DrawRect(Vector2i pos, Vector2i hSize, unsigned int color); 
    /* Fills the screen with color */
    void ClearScreen(unsigned int color); 
    /**
     * Draws a sprite on screen
     * Takes a bitmap, the size of the sprite, it's position and half size, 
     * the image offset and if the image needs to be reversed.
     * Used only in the Sprite class
     */
    void DrawSprite(Bitmap img, Vector2i spriteSize, Vector2i pos, Vector2i hSize, Vector2i offset, bool reversed);
    /* Updates the pixel buffer */
    void Update(float dt);

    /* Sets camera desired position */
    void TranslateCamera(Vector2i u);
    /* Returns current camera position */
    Vector2i GetCameraPos() const;

    /* Returns the pixel buffer's width */
    int GetBufferWidth() const;
    /* Returns the pixel buffer's height */
    int GetBufferHeight() const;

    std::vector<Sprite *> objectsToDraw;    // vector of all objects displayed on screen
    std::vector<Rect> rectsToDraw;          // vector of all rects displayed on screen
    std::vector<Text> textToDraw;           // vector of each text displayed on screen
    std::vector<Sprite *> uiToDraw;         // vector of all ui displayed on screen
    
    /* Sorts objects displayed by position on screen */
    void SortObjects();
};
