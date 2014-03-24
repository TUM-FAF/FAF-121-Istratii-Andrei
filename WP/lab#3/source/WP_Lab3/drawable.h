
#pragma once

#include <windows.h>


class Drawable
{
public:
    Drawable() { };
    virtual ~Drawable() { };

    virtual void Render(HDC hDC) = 0;
    virtual void Update();
};


class Rectangle : public Drawable
{

};

