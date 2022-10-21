#pragma once
#include "Vector.hpp"
#include "Renderer.hpp"

class Platform
{
private:
    Vector2i _pos;
    Vector2i _size;

public:
    Platform(Vector2i pos, Vector2i size);
    ~Platform();

    bool Collide(Vector2i playerPos, Vector2i playerSize);
    bool AroundPlatform(Vector2i playerPos);
    void Draw(Renderer *r);

    int Ground();
};

Platform::Platform(Vector2i pos, Vector2i size)
    : _pos(pos), _size(size)
{
}

Platform::~Platform()
{
}

bool Platform::Collide(Vector2i pp, Vector2i ps)
{
    return (pp.y-ps.y <= _pos.y+_size.y) && (pp.y+ps.y >= _pos.y-_size.y) &&
        (pp.x+ps.x >= _pos.x-_size.x) && (pp.x-ps.x <= _pos.x+_size.x);
}

bool Platform::AroundPlatform(Vector2i pp)
{
    return Distance(pp, _pos) <= (_size.x/2);
}

void Platform::Draw(Renderer *r)
{
    r->DrawRect(_pos, _size, 0x6169FF);
}

int Platform::Ground()
{
    return _pos.y+_size.y;
}