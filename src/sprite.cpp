/**
 * @file sprite.cpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * This file defines the Sprite class's implementation
 */

#include "Sprite.hpp"
#include "Window.hpp"
#include "Renderer.hpp"

//==============================================================================
// Sprite class implementation

Sprite::Sprite(Vector2i pos, float scale, Bitmap img, bool reversed, int offset)
    : _position(pos), _scale(scale), _img(img), _reversed(reversed)
{
    _size = {_img.width, _img.height};
    _centerOffset = offset;
}

Sprite::~Sprite()
{
}

void Sprite::Draw(Renderer *r)
{
    r->DrawSprite(_img, _size, _position, 
        _size*_scale, {0, 0}, _reversed);
}

void Sprite::IncrementFrameTime(float dt)
{
}

Vector2i Sprite::GetPosition() const
{
    return _position;
}

float Sprite::GetScale() const
{
    return _scale;
}

Bitmap Sprite::GetImage() const
{
    return _img;
}

Vector2i Sprite::GetImageSize() const
{
    return {_img.width, _img.height};
}

bool Sprite::IsReversed() const
{
    return _reversed;
}

int Sprite::GetPosOnGround() const
{
    return _position.y - (int)(_size.y*_scale) + (int)(_centerOffset*_scale*2);
}

void Sprite::SetPosition(Vector2i pos)
{
    _position = pos;
}

void Sprite::SetImage(Bitmap img) 
{
    _img = img;
}

void Sprite::SetReverse(bool reversed) 
{
    _reversed = reversed;
}

void Sprite::SetScale(float scale) 
{
    _scale = scale;
}

void Sprite::SetImageIndex(int index) 
{
}

//==============================================================================
// AnimatedSprite class implementation

AnimatedSprite::AnimatedSprite(Vector2i pos, float scale, std::vector<Animation> anim, bool reversed) 
    : Sprite(pos, scale, {0}, reversed), _animations(anim)
{
    _size = GetCurrentAnimation().GetImageDimensions();
    _frameTime = 1.f/GetCurrentAnimation().GetFps();
};

void AnimatedSprite::Draw(Renderer *r)
{
    // IncrementFrameTime();
    r->DrawSprite(GetCurrentAnimation().GetImage(_spriteIndex), _size, _position, 
        _size*_scale, {0, 0}, _reversed);
}

Animation AnimatedSprite::GetCurrentAnimation()
{
    return _animations.at(_animationIndex);
}

int AnimatedSprite::GetAnimationIndex() const
{
    return _animationIndex;
}

int AnimatedSprite::GetCurrentSpriteIndex() const
{
    return _spriteIndex;
}

void AnimatedSprite::IncrementFrameTime(float dt)
{
    _currentFrameTime += dt;
    if (_currentFrameTime > _frameTime)
    {
        int animationLength = GetCurrentAnimation().GetFrameLength()-1;
        _spriteIndex = (_spriteIndex < animationLength) ? _spriteIndex+1 : 0;
        if (!GetCurrentAnimation().IsLooping() && _spriteIndex == 0) 
        {
            if (GetCurrentAnimation().EndOnDefault())
            {
                _spriteIndex = 0;
                SetImageIndex(0);
            }
            else _spriteIndex = animationLength;
        }
            
        _currentFrameTime = 0;
    }
}

void AnimatedSprite::SetImageIndex(int index)
{
    if (index == _animationIndex)
        return;
    _currentFrameTime = 0;
    _spriteIndex = 0;
    _animationIndex = index;
    _frameTime = 1.f/GetCurrentAnimation().GetFps();
}

void AnimatedSprite::SetPosition(Vector2i pos)
{
    _position = pos;
}

//==============================================================================
// Timemap class implementation

Tilemap::Tilemap(Vector2i pos, float scale, SpriteSheet sprites, std::vector<int> map, Vector2i mapSize)
    : Sprite(pos, scale, {0}), _sprites(sprites), _map(map), _mapSize(mapSize)
{
    _size = {sprites.sWidth, sprites.sHeight};
    lastLayer = true;
}

void Tilemap::Draw(Renderer *r)
{
    for (int y = 0; y < _mapSize.y; y++)
    {
        for (int x = 0; x < _mapSize.x; x++)
        {
            Vector2i half_size = _size*_scale;
            Vector2i offset = {half_size.x*(1/2+x*2), half_size.y*(1/2+y*2)};
            if (_map[y*_mapSize.x+x] != -1)
                r->DrawSprite(_sprites.sheet, _size, _position+offset, half_size, 
                    {_map[y*_mapSize.x+x]%_sprites.sheet.width, _map[y*_mapSize.x+x]/_sprites.sheet.height}, _reversed);
        }
    }
}

SpriteSheet Tilemap::GetSpriteSheet() const
{
    return _sprites;
}

void Tilemap::SetPosition(Vector2i pos)
{
    _position = pos;
}

void Tilemap::SetMap(std::vector<int> map)
{
    _map = map;
}

void Tilemap::SetSpriteSheet(SpriteSheet sheet)
{
    _sprites = sheet;
}