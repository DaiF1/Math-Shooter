/**
 * @file FileSystem.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the engine's Animation class
 * It is used to describe an animation
 */

#pragma once
#include <vector>
#include <sstream>
#include "FileReader.hpp"
#include "Vector.hpp"

//==============================================================================
// Animation class
class Animation
{
private:
    std::vector<Bitmap> _images;    // images of the animation
    int _fps;                       // animation's fps
    int _frameLength;               // the number of frames in the animation
    bool _looping;                  // if the animation is looping
    bool _returnToDefault;          // if the animation returns to the default state when ended
    Vector2i _imageDimensions;      // individual images dimensions

public:
    /**
     * Constructor
     * Takes a path to the image's folder, the animation's fps,
     * if the animation loops and if it returns to the default state on end
     */
    Animation(const char *folderPath, int fps, bool looping, bool default = false);
    ~Animation();

    /* Returns if the animation loops */
    bool IsLooping() const;
    /* Returns true if the animations ends to the default */
    bool EndOnDefault() const;
    /* Returns the animation's length */
    int GetFrameLength() const;
    /* Returns the animation's fps */
    int GetFps() const;
    /* Returns the dimensions of each individual image */
    Vector2i GetImageDimensions() const;
    /* Returns an image at a given index */
    Bitmap GetImage(int index);
};

