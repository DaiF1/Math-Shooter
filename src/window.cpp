/**
 * @file window.cpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the Window class's implementation
 */

#include "Window.hpp"
#include <timeapi.h>
#include <sstream>

HWND Window::_window;
bool Window::_running = true;
bool Window::_active = true;
float Window::_freqCounter;
WinBuffer Window::_buffer;
unsigned int Window::_backgroundColor = 0x000000;

Window::Window(const char *name, int width, int height, HINSTANCE instance)
{
    WNDCLASSA winclass = {};
    winclass.style = CS_HREDRAW|CS_VREDRAW;
    winclass.lpfnWndProc = WindowProc;
    winclass.hInstance = instance;
    winclass.lpszClassName = "Default Window";

    RegisterClassA(&winclass);

    _window = CreateWindowExA(0, "Math Shooter", name, 
        WS_VISIBLE|WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX, 
        CW_USEDEFAULT,CW_USEDEFAULT, width, height, NULL, NULL, instance, NULL);


    if (_window == NULL)
        _running = false;

    _deviceContext = GetDC(_window);

    RECT rect;
    GetClientRect(_window, &rect);
    _buffer.width = rect.right - rect.left;
    _buffer.height = rect.bottom - rect.top;

    _buffer.pixels = (unsigned int *)VirtualAlloc(0, sizeof(unsigned int) * _buffer.width * _buffer.height,
                                                MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

    _buffer.info.bmiHeader.biSize = sizeof(_buffer.info.bmiHeader);
    _buffer.info.bmiHeader.biWidth = _buffer.width;
    _buffer.info.bmiHeader.biHeight = _buffer.height;
    _buffer.info.bmiHeader.biPlanes = 1;
    _buffer.info.bmiHeader.biBitCount = 32;
    _buffer.info.bmiHeader.biCompression = BI_RGB;

    QueryPerformanceCounter(&_lastCounter);
    LARGE_INTEGER freq_counter_large;
    QueryPerformanceFrequency(&freq_counter_large);
    _freqCounter = (float)freq_counter_large.QuadPart;
}

Window::~Window()
{
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
        case WM_DESTROY:
        {
            _running = false;
        } return 0;

        case WM_SIZING:
        {
            RECT rect;
            GetClientRect(_window, &rect);
            _buffer.width = rect.right - rect.left;
            _buffer.height = rect.bottom - rect.top;
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT paint;
			HDC deviceContext = BeginPaint(_window, &paint);

            RECT rect;
            GetClientRect(_window, &rect);
            HBRUSH brush = CreateSolidBrush(_backgroundColor);
            FillRect(deviceContext, &rect, brush);
            DeleteObject(brush);

            StretchDIBits(deviceContext, 0, 0, _buffer.width, _buffer.height, 
                0, 0, _buffer.width, _buffer.height, 
                _buffer.pixels, &_buffer.info, DIB_RGB_COLORS, SRCCOPY);

            EndPaint(_window, &paint);
        } return 0;

        case WM_EXITSIZEMOVE:
        {
            RECT rect;
            GetClientRect(_window, &rect);
            _buffer.width = rect.right - rect.left;
            _buffer.height = rect.bottom - rect.top;

            if (_buffer.pixels)
                VirtualFree(_buffer.pixels, 0, MEM_RELEASE);

            _buffer.pixels = (unsigned int *)VirtualAlloc(0, sizeof(unsigned int) * _buffer.width * _buffer.height,
                                                        MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

            _buffer.info.bmiHeader.biSize = sizeof(_buffer.info.bmiHeader);
            _buffer.info.bmiHeader.biWidth = _buffer.width;
            _buffer.info.bmiHeader.biHeight = _buffer.height;
            _buffer.info.bmiHeader.biPlanes = 1;
            _buffer.info.bmiHeader.biBitCount = 32;
            _buffer.info.bmiHeader.biCompression = BI_RGB;
        } return 0;
    }

    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

void Window::HandleMessages()
{
    _active = _window == GetForegroundWindow();

    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        switch (msg.message)
        {
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                if (!_active) return;

                unsigned int vk = (unsigned int)msg.wParam;
                bool down = (msg.lParam & (1 << 31)) == 0;

                if (vk == VK_UP)
                    input.buttons[kButtonUp].ProcessState(down);
                if (vk == VK_LEFT)
                    input.buttons[kButtonLeft].ProcessState(down);
                if (vk == VK_RIGHT)
                    input.buttons[kButtonRight].ProcessState(down);
                if (vk == VK_SPACE)
                    input.buttons[kButtonSpace].ProcessState(down);
            } return;
        
            default:
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } break;
        }
    }

    if (!_active) return;
    input.buttons[kButtonLeft].ProcessState(KEY_DOWN(VK_LEFT));
    input.buttons[kButtonRight].ProcessState(KEY_DOWN(VK_RIGHT));
    input.buttons[kButtonUp].ProcessState(KEY_DOWN(VK_UP));
    input.buttons[kButtonSpace].ProcessState(KEY_DOWN(VK_SPACE));
}

void Window::ProcessFrame()
{
    RECT rect;
    GetClientRect(_window, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    StretchDIBits(_deviceContext, 0, 0, width, height, 
        0, 0, _buffer.width, _buffer.height, 
        _buffer.pixels, &_buffer.info, DIB_RGB_COLORS, SRCCOPY);

    float ft = min(.1f, GetElapsedTime());
    int sleepTime = (int)(1000.f * (_targetFt - ft));

    timeBeginPeriod(1);
    if (sleepTime > 1)
        Sleep(sleepTime-1);
    timeEndPeriod(1);

    ft = GetElapsedTime();
    _lastFt = (_active) ? ft : 0.f;
    
    QueryPerformanceCounter(&_lastCounter);
}

void Window::SetBackgroundColor(unsigned int color) const
{
    _backgroundColor = color;
}

bool Window::IsRunning() const
{
    return _running;
}

HWND *Window::GetWindow() const
{
    return &_window;
}

float Window::GetElapsedTime() const
{
    LARGE_INTEGER currentCounter;
    QueryPerformanceCounter(&currentCounter);
    return (float)(currentCounter.QuadPart - _lastCounter.QuadPart) / _freqCounter;
}

float Window::GetFt() const
{
    return _lastFt;
}

WinBuffer *Window::GetBuffer() const
{
    return &_buffer;
}