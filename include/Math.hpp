/**
 * @file Math.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines a collection of different math functions
 */

#pragma once
#include <math.h>
#include <vector>

//==============================================================================
// General math functions

#define PI 3.14159265359f

/* Returns the absolute value of a */
inline float Abs(float a)
{
    if (a < 0) return -a;
    return a;
}

/* Returns the square of x */
inline float Square(float x)
{
    return x*x;
}

//==============================================================================
// Value limitation

/* Clamp a value between a min and a max. Only takes integers */
inline int Clamp(int min, int val, int max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

/* Clamp a value between a min and a max. Only takes floats */
inline float Clampf(float min, float val, float max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

//==============================================================================
// Interpolation

/* Linearly interpolate between a and b by w */
inline float Lerp(float a, float b, float w)
{
    return (1-w)*a + w*b;
}

/* Move value to target by factor w */
inline float MoveTo(float val, float target, float w)
{
    if (val > target) return Clampf(target, val-w, val);
    if (val < target) return Clampf(val, val+w, target);
    return val;
}

/* Interpolates linearly values between d0 and d1 by a precision going from i0 to i1 */
inline std::vector<float> Interpolate(const int i0, float d0, const int i1, float d1)
{
    if (i0 == i1) return {d0};

    std::vector<float> result;
    float a = (d1-d0)/(float)(i1-i0);
    float d = d0;
    for (int i = i0; i <=i1; i++)
    {
        result.push_back(d);
        d += a;
    }

    return result;
}