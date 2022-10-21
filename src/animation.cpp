/**
 * @file FileSystem.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the engine's animation system and implementation
 */

#include "Animation.hpp"
#include "Vector.hpp"

Animation::Animation(const char *folderPath, int fps, bool looping, bool default)
    : _fps(fps), _looping(looping), _returnToDefault(default)
{
    for (int i = 0; i < NumberOfFiles(folderPath); i++)
    {
        std::stringstream file;
        file << folderPath << "\\" << i << ".png";
        _images.push_back(ReadImage(file.str().c_str()));
    }
    _imageDimensions = {_images.at(0).width, _images.at(0).height};
    _frameLength = _images.size();
}

Animation::~Animation()
{
}

bool Animation::IsLooping() const
{
    return _looping;
}

bool Animation::EndOnDefault() const
{
    return _returnToDefault;
}

int Animation::GetFrameLength() const
{
    return _frameLength;
}

int Animation::GetFps() const
{
    return _fps;
}

Vector2i Animation::GetImageDimensions() const
{
    return _imageDimensions;
}

Bitmap Animation::GetImage(int index)
{
    return _images.at(index);
}