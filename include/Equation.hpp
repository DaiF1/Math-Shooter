#pragma once
#include "Vector.hpp"
#include "Sprite.hpp"

class Equation
{
private:
    Vector2i _pos;
    Sprite *_sprite;
    int _life;
    int _pv;

public:
    Equation(Vector2i pos, int life, const char *imgPath);
    ~Equation();

    void Update();
    void GetHit();
    void Restart();
    bool OutOfBound();

    Sprite *GetSprite();
    Vector2i GetPos();
    Vector2i GetSize();
};

Equation::Equation(Vector2i pos, int life, const char *imgPath)
    : _pos(pos), _life(life), _pv(life)
{
    _sprite = new Sprite(pos, .15f, ReadImage(imgPath));
}

Equation::~Equation()
{
}

Sprite *Equation::GetSprite()
{
    return _sprite;
}

void Equation::Update()
{
    _pos.x -= 1;
    _sprite->SetPosition(_pos);
}

void Equation::GetHit()
{
    _pv--;
    if (_pv<=0)
        Restart();
}

void Equation::Restart()
{
    _pos.x = 1200+(GetSize().x+10)*2;
    _pos.y = (rand() % 3)*200 + 100;
    _pv = _life;
}

bool Equation::OutOfBound()
{
    return _pos.x + GetSize().x/2 < 0;
}

Vector2i Equation::GetPos()
{
    return _pos;
}

Vector2i Equation::GetSize()
{
    return _sprite->GetImageSize()*_sprite->GetScale();
}