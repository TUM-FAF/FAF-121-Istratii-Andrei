
#pragma once

#include <vector>
#include <windows.h>

#include "canvas.h"
#include "mouse.h"
#include "drawable.h"

class Drawer : public IMouseListener
{
public:
    Drawer();
    ~Drawer();
    virtual void OnLeftMouseButtonDown(int x, int y);
    virtual void OnLeftMouseButtonUp(int x, int y);
    virtual void OnMouseMove(int x, int y);

    void Render(HDC hDC);

    void LinkToCanvas(Canvas* canvas);

private:
    bool isDrawing;
    Drawable* tempObject;
};