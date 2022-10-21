/**
 * @file Bullet.hpp
 * @author Julie Fiadino
 * @copyright Copyright 2021 (C) Julie Fiadino
 * 
 * Bullets implementation
 */

#pragma once

#include "Renderer.hpp"
#include "Vector.hpp"

class Bullet
{
private:
    Vector2i _pos;
    Vector2i _hSize;
    bool _instantiated; 
    int _dir;
public: 
    Bullet(Vector2i pos, int dir);
    void Update(Renderer *r);
    void Draw(Renderer *r);
    bool NotOnScreen();
    bool Hit(Vector2i pp, Vector2i ps);
};

Bullet::Bullet(Vector2i pos, int dir)
    : _pos(pos), _dir(dir)
{
    _hSize = {3,2};
    _instantiated = true;
}

void Bullet::Update(Renderer *r)
{
    _pos.x += 10*_dir; 
    if (_pos.x+_hSize.x >= r->GetBufferWidth() || _pos.x-_hSize.x < 0) 
        _instantiated = false;
}

void Bullet::Draw(Renderer *r)
{
    if (_instantiated) 
        r->DrawRect(_pos, _hSize, 0xE6C440);
}

bool Bullet::NotOnScreen()
{
    return !_instantiated;
}

bool Bullet::Hit(Vector2i pp, Vector2i ps)
{
    return (pp.y-ps.y <= _pos.y+2) && (pp.y+ps.y >= _pos.y-2) &&
        (pp.x+ps.x >= _pos.x-10*2*_dir) && (pp.x-ps.x <= _pos.x-10*2*_dir);
}