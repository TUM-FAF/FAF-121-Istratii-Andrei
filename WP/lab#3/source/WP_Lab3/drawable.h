
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



class StartEndFigure : public Drawable
{
public:
    StartEndFigure(int x, int y);
    virtual void Update(int x, int y);

protected:
    POINT start;
    POINT end;
};


class Rect : public StartEndFigure
{
public:
    Rect(int x, int y);

    virtual void Render(HDC hDC);
};


class Elips : public StartEndFigure
{
public:
    Elips(int x, int y);

    virtual void Render(HDC hDC);
};

