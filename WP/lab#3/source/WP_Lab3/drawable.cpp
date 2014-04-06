
#include "drawable.h"
#include "shared_headers.h"


Drawable::Drawable(HBRUSH hbr, HPEN hp)
{
    hBrush = hbr;
    hPen = hp;
}


Drawable::~Drawable()
{
    DeleteObject(hBrush);
    DeleteObject(hPen);
}


void Drawable::SetPenAndBrush(HDC hDC)
{
    SelectObject(hDC, hBrush);
    SelectObject(hDC, hPen);
}


void Drawable::Render(HDC hDC)
{
    SetPenAndBrush(hDC);
    Draw(hDC);
}


StartEndFigure::StartEndFigure(HBRUSH hbr, HPEN hp, int x, int y)
    : Drawable(hbr, hp)
{
    start.x = x;
    start.y = y;

    end.x = x;
    end.y = y;
}


void StartEndFigure::Update(int x, int y)
{
    end.x = x;
    end.y = y;
}




Rect::Rect(HBRUSH hbr, HPEN hp, int x, int y)
    : StartEndFigure(hbr, hp, x, y) { }


void Rect::Draw(HDC hDC)
{
    Rectangle(hDC, start.x, start.y, end.x, end.y);
}


Elips::Elips(HBRUSH hbr, HPEN hp, int x, int y)
    : StartEndFigure(hbr, hp, x, y) { }

void Elips::Draw(HDC hDC)
{
    Ellipse(hDC, start.x, start.y, end.x, end.y);
}

