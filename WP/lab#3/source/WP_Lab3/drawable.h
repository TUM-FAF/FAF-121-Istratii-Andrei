
#pragma once

#include <windows.h>


class Drawable
{
public:
    Drawable(HBRUSH hbr, HPEN hp);
    virtual ~Drawable();

    void SetPenAndBrush(HDC hDC);
    void Render(HDC hDC);
    virtual void Draw(HDC hDC) = 0;
    virtual void Update(int x, int y, bool constraint) = 0;

private:
    HBRUSH hBrush;
    HPEN hPen;
};



class StartEndFigure : public Drawable
{
public:
    StartEndFigure(HBRUSH hbr, HPEN hp, int x, int y);
    virtual void Update(int x, int y, bool constraint);

protected:
    POINT start;
    POINT end;
};


class Rect : public StartEndFigure
{
public:
    Rect(HBRUSH hbr, HPEN hp, int x, int y);

    virtual void Draw(HDC hDC);
};


class Elips : public StartEndFigure
{
public:
    Elips(HBRUSH hbr, HPEN hp, int x, int y);

    virtual void Draw(HDC hDC);
};

