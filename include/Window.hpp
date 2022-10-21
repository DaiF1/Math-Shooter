/**
 * @file Window.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the Window class along with the WinBuffer struct
 * It is used to display a Window
 */

#pragma once
#include <Windows.h>

#include "Input.hpp"

//==============================================================================
// The window pixel buffer
struct WinBuffer
{
    int width, height;      // width and height of the screen
    unsigned int *pixels;   // color of each pixel
    BITMAPINFO info;        // WinApi Bitmap info
};

//==============================================================================
// Window class
class Window
{
private:
    static HWND _window;                    // WinApi window
    HDC _deviceContext;                     // used to display buffer on screen

    LARGE_INTEGER _lastCounter;             // last frame time counter. used to calculate framerate
    static float _freqCounter;              // time check frequency. used to calculate framerate
    float _targetFt = 0.01666f;             // ideal frame time. used to lock the framerate
    float _lastFt;                          // last frame's time
    static bool _active;                    // used to stop activities when the window isn't active

    static bool _running;                   // used to stop the program
    static LRESULT CALLBACK                 // WinApi callback. processes window messages
        WindowProc(HWND hwnd, UINT uMsg, 
            WPARAM wParam, LPARAM lParam);

    static WinBuffer _buffer;               // pixel buffer
    static unsigned int _backgroundColor;   // defines the color used when stretching the window
    
    /* Returns the elapsed time since last frame */
    float GetElapsedTime() const;

public:
    /**
     * Constructor
     * Takes name, dimensions and instance of the window
     */
    Window(const char *name, int width, int height, HINSTANCE instance);
    ~Window();

    /* Processes user messages */
    void HandleMessages();
    /* Displays frame on framerate */ 
    void ProcessFrame(); 

    /* Sets background color */
    void SetBackgroundColor(unsigned int color) const;

    /* Returns true if the program is currently running */
    bool IsRunning() const; 
    /* Returns a pointer to the WinApi Window */
    HWND *GetWindow() const; 
    /* Returns a pointer to the pixel buffer */
    WinBuffer *GetBuffer() const;

    /** Returns last frame time. 
     * Can be used as a delta time to lock movement to framerate 
     */
    float GetFt() const;

    Input input;    // user input
};