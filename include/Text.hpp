/**
 * @file Text.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines Text objects and logic
 */

#pragma once
#include "FileReader.hpp"

//==============================================================================
// The different types of text alignement
enum TextAlign
{
    kAlignLeft,
    kAlignRight,
    kAlignCenter,
};

// Defines a text font
struct Font
{
    Bitmap bitmap;          // image file displayed on screen
    int lWidth, lHeight;    // singular letter dimensions
};

// Defines a text object
struct Text
{
    char *text;             // text string
    Font font;              // text font
    Vector2i pos;           // text position
    int size;               // text size
    int textAlign;          // text alignement
};

//==============================================================================
/* Returns letter index in table */
inline int GetLetterIndex(char c) 
{
    if (c == '.') return 'Z'-'A'+1;
    if (c == ',') return 'Z'-'A'+2;
    if (c == '\'') return 'Z'-'A'+3;
    if (c == '!') return 'Z'-'A'+4;
    return c-'A';
}

/* Returns offset based on text align */
static float GetWordAlignOffset(int textAlign, char *word, float size, int spacing) 
{
    if (textAlign == kAlignLeft) return 0.f;
    
    float result = 0.f;
    float off = size*2.f;
    
    for (char *at = word; *at && *at != '\\'; at++) 
    {
        if (*at == ' ') 
        {
            result += off*5;
            continue;
        }
        
        int letter_index = GetLetterIndex(*at);
        result += off*spacing + off/4;
    }
    
    result -= off;
    
    if (textAlign == kAlignRight) 
        return -result;
    return -result*.5f;
}

/* Load font from file path. Returns Font object */
static Font ReadFont(char *path)
{
    Bitmap img = ReadImage(path);
    Font result = {0};
    result.bitmap = img; 
    result.lWidth = img.width/30;
    result.lHeight = img.height;
    return result;
}