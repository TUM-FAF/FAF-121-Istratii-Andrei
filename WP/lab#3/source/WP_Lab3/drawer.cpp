
#include "drawer.h"


Drawer::Drawer()
{
    isDrawing = false;
}


Drawer::~Drawer()
{

}


void Drawer::OnLeftMouseButtonDown(int x, int y)
{
    if (!isDrawing)
    {
        POINT p;
        p.x = x;
        p.y = y;
        points.clear();
        points.push_back(p);
        points.push_back(p);
        isDrawing = true;
    }
}


void Drawer::OnLeftMouseButtonUp(int x, int y)
{
    if (isDrawing)
    {
        isDrawing = false;
    }
}


void Drawer::OnMouseMove(int x, int y)
{
    if (isDrawing)
    {
        points.back().x = x;
        points.back().y = y;
    }
}


void Drawer::Render(HDC hDC)
{
    if (isDrawing)
    {
        Rectangle(hDC, points.front().x, points.front().y, points.back().x, points.back().y);
    }
}

