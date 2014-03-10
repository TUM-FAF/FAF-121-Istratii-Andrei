#pragma once

#include <Windows.h>


class Canvas
{
public:
    Canvas(HDC hWndDC, int w, int h);
    ~Canvas();

private:
    HDC hDC;
    HBITMAP hBMP;
    HBITMAP hOldBMP;

    RECT clippingRect;

    int width;
    int height;
};