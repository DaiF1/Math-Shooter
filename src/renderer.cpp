/**
 * @file renderer.cpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the Renderer class's implementation
 */

#include "Renderer.hpp"
#include "Math.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
#include <algorithm>

Renderer::Renderer(Window win)
{
    _buffer = win.GetBuffer();
}

Renderer::~Renderer()
{
}

void Renderer::ClearScreen(unsigned int color)
{
    unsigned int *pixel = _buffer->pixels;
    for (int y = 0; y < _buffer->height; y++)
    {
        for (int x = 0; x < _buffer->width; x++)
        {
            *pixel++ = color;
        }
    }
}

void Renderer::DrawRect(Vector2i pos, Vector2i hSize, unsigned int color)
{
    int xmin = max(0, pos.x-hSize.x);
    int xmax = min(_buffer->width, pos.x+hSize.x);
    int ymin = max(0, pos.y-hSize.y);
    int ymax = min(_buffer->height, pos.y+hSize.y);

    for (int y = ymin; y < ymax; y++)
    {
        unsigned int *pixel = _buffer->pixels + y*_buffer->width + xmin;
        for (int x = xmin; x < xmax; x++)
        {
            *pixel++ = color;
        }
    }
}

void Renderer::DrawRoundedRect(Vector2i pos, Vector2i hSize, int radius, unsigned int color)
{
    int xmin = max(0, pos.x-hSize.x);
    int xmax = min(_buffer->width, pos.x+hSize.x);
    int ymin = max(0, pos.y-hSize.y);
    int ymax = min(_buffer->height, pos.y+hSize.y);

    for (int y = ymin; y < ymax; y++)
    {
        unsigned int *pixel = _buffer->pixels + y*_buffer->width + xmin;
        for (int x = xmin; x < xmax; x++)
        {
            if ((y < ymin+radius || y >= ymax-radius-1) && (x < xmin+radius || x >= xmax-radius-1))
            {
                int w = xmax-xmin;
                int h = ymax-ymin;
                int mx = (xmin+xmax)/2;
                int my = (ymin+ymax)/2;

                Vector2i corner = {
                    w*(x > mx) + radius*((x < mx)*2 - 1) - 1*(x > mx), 
                    h*(y > my) + radius*((y < my)*2 - 1) - 1*(y > my)
                };

                if (Distance(corner, {x-xmin, y-ymin}) > radius)
                {
                    pixel++;
                    continue;
                }
            }
            *pixel++ = color;
        }
    }   
}

void Renderer::DrawLetter(Font *font, int index, Vector2i pos, Vector2i hSize)
{
    int x0 = (int)(pos.x-hSize.x)-_cam.x;
    int y0 = (int)(pos.y-hSize.y)-_cam.y;
    int x1 = (int)(pos.x+hSize.x)-_cam.x;
    int y1 = (int)(pos.y+hSize.y)-_cam.y;

    float xRange = (float)(x1 - x0);
    float yRange = (float)(y1 - y0);
    
    x0 = Clamp(0, x0, _buffer->width);
    x1 = Clamp(0, x1, _buffer->width);
    y0 = Clamp(0, y0, _buffer->height);
    y1 = Clamp(0, y1, _buffer->height);
    
    unsigned int *row = _buffer->pixels + x0 + _buffer->width*y0;
    unsigned int *pixel = row;
    int stride = _buffer->width;
    for (int y = y0; y < y1; y++) 
    {  
        float v = ((float)y-y0)/yRange;
        int uvStride = (int)(v*(float)font->lHeight)*font->bitmap.width;
        unsigned int *src_pixels = font->bitmap.pixels + uvStride;
        for (int x = x0; x < x1; x++) 
        {
            float u = ((float)x-x0)/xRange;
            
            int px = (int)(u*(float)font->lWidth)+index*font->lWidth;
            
            unsigned int bmpColor = *(src_pixels + px);
            
            float alpha = (float)((bmpColor & 0xff000000) >> 24)/255.f;
            
            float ar = (float)((*pixel & 0xff0000) >> 16);
            float ag = (float)((*pixel & 0xff00) >> 8);
            float ab = (float)(*pixel & 0xff);
            
            float br = (float)((bmpColor & 0xff0000) >> 16);
            float bg = (float)((bmpColor & 0xff00) >> 8);
            float bb = (float)(bmpColor & 0xff);
            
            unsigned char r = (unsigned char)((1-alpha)*ar + alpha*br);
            unsigned char g = (unsigned char)((1-alpha)*ag + alpha*bg);
            unsigned char b = (unsigned char)((1-alpha)*ab + alpha*bb);
            
            unsigned int pixel_color = b | (g << 8) | (r << 16);
            *pixel++ = pixel_color;
        }
        row += stride;
        pixel = row;
    }
}

void Renderer::PrintText(Text text)
{
    int fx = text.pos.x;

    int ox = text.pos.x;
    int oy = text.pos.y;
    Vector2i hSize = {text.size*text.font.lWidth, text.size*text.font.lHeight};
    int off = text.size*text.font.lWidth;

    for (char *lett = text.text; *lett; lett++) 
    {
        if (*lett == ' ') 
        {
            text.pos.x += off*3;
            ox = text.pos.x;
            continue;
        }
        else if (*lett == '\\') 
        {
            text.pos.x = fx;
            text.pos.x += (int)GetWordAlignOffset(text.textAlign, lett+1, (float)text.size, text.font.lWidth/2);
            ox = text.pos.x;
            text.pos.y -= (int)(text.size*(text.font.lHeight+2)*2.5f);
            oy = text.pos.y;
            continue;
        }
        
        int letterIndex = GetLetterIndex(*lett);
        DrawLetter(&text.font, letterIndex, text.pos, hSize);
        
        text.pos.x = ox;
        text.pos.y = oy;
        text.pos.x += off*2;
        ox = text.pos.x;
    }
}

void Renderer::DrawSprite(Bitmap img, Vector2i sprite_size, Vector2i pos, Vector2i hSize, Vector2i offset, bool reversed)
{
    int x0 = (int)(pos.x-hSize.x)-_cam.x;
    int y0 = (int)(pos.y-hSize.y)-_cam.y;
    int x1 = (int)(pos.x+hSize.x)-_cam.x;
    int y1 = (int)(pos.y+hSize.y)-_cam.y;

    float xRange = (float)(x1 - x0);
    float yRange = (float)(y1 - y0);

    int x0Clamped = Clamp(0, x0, _buffer->width);
    int y0Clamped = Clamp(0, y0, _buffer->height);
    x1 = Clamp(0, x1, _buffer->width);
    y1 = Clamp(0, y1, _buffer->height);

    unsigned int *row = _buffer->pixels + x0Clamped + _buffer->width*y0Clamped;
    unsigned int *pixel = row;
    int stride = _buffer->width;
    for (int y = y0Clamped; y < y1; y++) 
    {  
        float v = ((float)y-y0Clamped)/yRange + (float)(y0Clamped-y0)/yRange;
        int uvStride = (int)((v+(float)offset.y)*(float)sprite_size.y)*img.width;
        unsigned int *src_pixels = img.pixels + uvStride;
        for (int x = x0Clamped; x < x1; x++) 
        {
            float u = ((float)x-x0Clamped)/xRange + (float)(x0Clamped-x0)/xRange; 
            int px = 0;
            if (!reversed)
                px = (int)(u*(float)sprite_size.x)+offset.x*sprite_size.x;
            else 
                px = (offset.x+1)*sprite_size.x-(int)(u*(float)sprite_size.x)-1;
            
            unsigned int bmpColor = *(src_pixels + px);
            
            float alpha = (float)((bmpColor & 0xff000000) >> 24)/255.f;
            
            float ar = (float)((*pixel & 0xff0000) >> 16);
            float ag = (float)((*pixel & 0xff00) >> 8);
            float ab = (float)(*pixel & 0xff);
            
            float br = (float)((bmpColor & 0xff0000) >> 16);
            float bg = (float)((bmpColor & 0xff00) >> 8);
            float bb = (float)(bmpColor & 0xff);
            
            unsigned char r = (unsigned char)((1-alpha)*ar + alpha*br);
            unsigned char g = (unsigned char)((1-alpha)*ag + alpha*bg);
            unsigned char b = (unsigned char)((1-alpha)*ab + alpha*bb);
            
            unsigned int pixel_color = b | (g << 8) | (r << 16);
            *pixel++ = pixel_color;
        }
        row += stride;
        pixel = row;
    }
}

void Renderer::Update(float dt)
{
    if (!(_desiredCam.x == _cam.x && _desiredCam.y == _cam.y))
    {
        _transitionTime += dt/2;
        if (_transitionTime > 1) _transitionTime = 1;
        _cam.x = (int)Lerp((float)_cam.x, (float)_desiredCam.x, _transitionTime);
        _cam.y = (int)Lerp((float)_cam.y, (float)_desiredCam.y, _transitionTime);
    }
    else
        _transitionTime = 0;

    for (const Rect &r : rectsToDraw)
        DrawRect(r.pos-_cam, r.hSize, r.color);
    for (auto const &o : objectsToDraw)
    {
        o->IncrementFrameTime(dt);
        o->Draw(this);
    }
    for (const Text &t : textToDraw)
        PrintText(t);
    for (auto const &u : uiToDraw)
        u->Draw(this);
}

void Renderer::TranslateCamera(Vector2i u)
{
    _desiredCam += u;
}

Vector2i Renderer::GetCameraPos() const
{
    return _cam;
}

int Renderer::GetBufferWidth() const
{
    return _buffer->width;
}

int Renderer::GetBufferHeight() const
{
    return _buffer->height;
}

void Renderer::SortObjects()
{
    std::sort(objectsToDraw.begin(), objectsToDraw.end(), []
        (const Sprite *lhs, const Sprite *rhs)
        {
            if (lhs->lastLayer) return true;
            if (rhs->lastLayer) return false;
            return lhs->GetPosOnGround() > rhs->GetPosOnGround();
        }
    );
}