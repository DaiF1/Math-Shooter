/**
 * @file Vector.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the Vector2 object
 */


#pragma once
#include <math.h>
#include "Math.hpp"

//==============================================================================
// Vector2 int
struct Vector2i
{
    int x, y;

    friend Vector2i operator+(Vector2i self, Vector2i &other)
    {
        return {self.x+other.x, self.y+other.y};
    }   
    friend Vector2i operator-(Vector2i self, Vector2i &other)
    {
        return {self.x-other.x, self.y-other.y};
    }
    friend Vector2i operator/(Vector2i self, const float &k)
    {
        return {(int)(self.x/k), (int)(self.y/k)};
    }
    friend Vector2i operator*(Vector2i self, const float &k)
    {
        return {(int)(self.x*k), (int)(self.y*k)};
    }

    void operator+=(Vector2i &other)
    {
        x += other.x;
        y += other.y;
    }
    void operator-=(Vector2i &other)
    {
        x -= other.x;
        y -= other.y;
    }
    void operator*=(float &k)
    {
        x *= (int)k;
        y *= (int)k;
    }
    void operator/=(float &k)
    {
        x /= (int)k;
        y /= (int)k;
    }
};

//==============================================================================
// Vector2 float
struct Vector2f
{
    float x, y;

    friend Vector2f operator+(Vector2f self, Vector2f &other)
    {
        return {self.x+other.x, self.y+other.y};
    }
    friend Vector2f operator-(Vector2f self, Vector2f &other)
    {
        return {self.x-other.x, self.y-other.y};
    }
    friend Vector2f operator+(Vector2f self, Vector2i &other)
    {
        return {self.x+other.x, self.y+other.y};
    }
    friend Vector2f operator-(Vector2f self, Vector2i &other)
    {
        return {self.x-other.x, self.y-other.y};
    }
    friend Vector2f operator*(Vector2f self, const float &k)
    {
        return {self.x*k, self.y*k};
    }
    friend Vector2f operator/(Vector2f self, const float &k)
    {
        return {self.x/k, self.y/k};
    }

    void operator+=(Vector2f &other)
    {
        x += other.x;
        y += other.y;
    }
    void operator-=(Vector2f &other)
    {
        x -= other.x;
        y -= other.y;
    }
    void operator+=(Vector2i &other)
    {
        x += other.x;
        y += other.y;
    }
    void operator-=(Vector2i &other)
    {
        x -= other.x;
        y -= other.y;
    }
    void operator*=(float &k)
    {
        x *= k;
        y *= k;
    }
    void operator/=(float &k)
    {
        x /= k;
        y /= k;
    }
};

//==============================================================================
/* Returns the magnitude of a vector u */
inline float Magnitude(Vector2i u)
{
    return sqrtf((float)(u.x*u.x+u.y*u.y));
}

/* Returns the norm of a vector u */
inline Vector2f Normalize(Vector2i u)
{
    return {(u.x/Magnitude(u)), (u.y/Magnitude(u))};
}

/* Returns the distance between two vectors */
inline float Distance(Vector2i a, Vector2i b)
{
    float x = Square((float)(b.x-a.x));
    float y = Square((float)(b.y-a.y));
    return sqrtf(x + y);
}