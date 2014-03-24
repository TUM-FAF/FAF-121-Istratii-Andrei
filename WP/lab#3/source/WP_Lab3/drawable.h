
#pragma once

#include <windows.h>


class Drawable
{
public:
    Drawable() { };
    virtual ~Drawable() { };

    virtual void Render(HDC hDC) = 0;
    virtual void Update(int x, int y) = 0;
};


class Rect : public Drawable
{
public:
    Rect(int x, int y);

    virtual void Render(HDC hDC);
    virtual void Update(int x, int y);

private:
    POINT start;
    POINT end;
};

