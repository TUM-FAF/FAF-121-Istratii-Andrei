
#include "drawer.h"
#include "shared_headers.h"



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
        tempObject = new Rect(x, y);
        std::cout << "create rect\n";
        isDrawing = true;
    }
}


void Drawer::OnLeftMouseButtonUp(int x, int y)
{
    if (isDrawing)
    {
        std::cout << "delete rect\n";
        delete tempObject;
        isDrawing = false;
    }
}


void Drawer::OnMouseMove(int x, int y)
{
    if (isDrawing)
    {
        std::cout << "update rect\n";
        tempObject->Update(x, y);
    }
}


void Drawer::Render(HDC hDC)
{
    if (isDrawing)
    {
        std::cout << "render rect\n";
        tempObject->Render(hDC);
    }
}

