#pragma once

#include <Windows.h>


class Canvas
{
public:
    Canvas();
    ~Canvas();

    void Init(HDC hWndDC, int w, int h);
    HDC GetDC() { return hDC; }
    void Zoom(float z, int mx, int my, int vw, int vh);
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

    void AdjustPanLimits(int vw, int vh);
};