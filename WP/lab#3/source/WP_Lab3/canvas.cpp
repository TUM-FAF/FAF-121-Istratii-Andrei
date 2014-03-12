
#include "canvas.h"
#include <iostream>


Canvas::Canvas()
{
    hDC = NULL;
    hBMP = NULL;
    hOldBMP = NULL;

    width = 0;
    height = 0;
}


Canvas::~Canvas()
{
    // temporary



    if (hOldBMP) { SelectObject(hDC, hOldBMP); }
    if (hBMP) { DeleteObject(hBMP); }
    if (hDC) { DeleteDC(hDC); }
}


void Canvas::Init(HDC hWndDC, int w, int h)
{
    hDC = CreateCompatibleDC(hWndDC);
    hBMP = CreateCompatibleBitmap(hWndDC, w, h);
    hOldBMP = (HBITMAP)SelectObject(hDC, hBMP);

    width = w;
    height = h;

    // temporary
    HDC hDukeDC = CreateCompatibleDC(hDC);
    HBITMAP hDukeBMP = (HBITMAP)LoadImage(NULL, L"..\\..\\assets\\duke.bmp", IMAGE_BITMAP, 685, 610, LR_LOADFROMFILE);
    std::cout << hDukeBMP << " (duke bmp)\n";
    HBITMAP hDukeOldBMP = (HBITMAP)SelectObject(hDukeDC, hDukeBMP);

    BitBlt(hDC, 0, 0, 685, 610, hDukeDC, 0, 0, SRCCOPY);

    DeleteObject(SelectObject(hDukeDC, hDukeOldBMP));
    DeleteDC(hDukeDC);
    // end temporary
}