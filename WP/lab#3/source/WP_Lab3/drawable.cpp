
#include "drawable.h"
#include "shared_headers.h"


StartEndFigure::StartEndFigure(int x, int y)
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


 
Rect::Rect(int x, int y) : StartEndFigure(x, y) { }

void Rect::Render(HDC hDC)
{
    Rectangle(hDC, start.x, start.y, end.x, end.y);
}


Elips::Elips(int x, int y) : StartEndFigure(x, y) { }

void Elips::Render(HDC hDC)
{
    Ellipse(hDC, start.x, start.y, end.x, end.y);
}

