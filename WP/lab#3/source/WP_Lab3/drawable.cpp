
#include "drawable.h"


Rect::Rect(int x, int y)
{
    start.x = x;
    start.y = y;

    end.x = x;
    end.y = y;
}


void Rect::Render(HDC hDC)
{
    Rectangle(hDC, start.x, start.y, end.x, end.y);
}


void Rect::Update(int x, int y)
{
    end.x = x;
    end.y = y;
}