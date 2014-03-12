#pragma once

#include <Windows.h>


class Canvas
{
public:
    Canvas();
    ~Canvas();

    void Init(HDC hWndDC, int w, int h);
    HDC GetDC() { return hDC; }

private:
    HDC hDC;
    HBITMAP hBMP;
    HBITMAP hOldBMP;

    RECT clippingRect;

    int width;
    int height;
};