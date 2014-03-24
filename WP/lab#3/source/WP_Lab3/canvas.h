#pragma once

#include <windows.h>
#include <list>
#include "drawable.h"


class Canvas
{
public:
    Canvas();
    ~Canvas();

    void Init(HDC hWndDC, int w, int h);
    HDC GetDC() { return hDC; }
    void StepZoomIn(int mx, int my, int vw, int vh);
    void StepZoomOut(int mx, int my, int vw, int vh);
    void Zoom(int mx, int my, int vw, int vh);
    void Pan(int dx, int dy, int vw, int vh);

    int GetWidth() { return width; }
    int GetHeight() { return height; }
    RECT GetZoomRect() { return zoomRect; }

private:
    HDC hDC;
    HBITMAP hBMP;
    HBITMAP hOldBMP;

    RECT zoomRect;

    int width;
    int height;

    float zoomFactor;

    std::list<Drawable*> objects;

    void AdjustPanLimits(int vw, int vh);

    float NextZoomStep();
    float PrevZoomStep();

    static float const zoomSteps[25];
};
