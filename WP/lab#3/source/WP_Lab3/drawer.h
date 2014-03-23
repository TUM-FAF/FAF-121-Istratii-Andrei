
#pragma once

#include <vector>
#include <windows.h>

#include "mouse.h"

class Drawer : public IMouseListener
{
public:
    Drawer();
    ~Drawer();
    virtual void OnLeftMouseButtonDown(int x, int y);
    virtual void OnLeftMouseButtonUp(int x, int y);
    virtual void OnMouseMove(int x, int y);

    void Render(HDC hDC);


protected:
    bool isDrawing;
    std::vector<POINT> points;

private:
};