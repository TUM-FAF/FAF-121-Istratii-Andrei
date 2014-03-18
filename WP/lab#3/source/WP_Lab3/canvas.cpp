
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

    zoomRect.left = 0;
    zoomRect.top = 0;
    zoomRect.right = width;
    zoomRect.bottom = height;


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


void Canvas::Pan(int dx, int dy, int vw, int vh)
{
    zoomRect.left += dx;
    zoomRect.top += dy;
    zoomRect.right += dx;
    zoomRect.bottom += dy;

    AdjustPanLimits(vw, vh);
}


void Canvas::Zoom(float z, int mx, int my, int vw, int vh)
{
    int w = zoomRect.right - zoomRect.left;
    int h = zoomRect.bottom - zoomRect.top;

    RECT margin;

    zoomRect.left = z*(mx - zoomRect.left) - mx;
    zoomRect.top = z*(my - zoomRect.top) - my;
    zoomRect.right = z*(zoomRect.right - mx) + mx;
    zoomRect.bottom = z*(zoomRect.bottom - my) + my;

    //AdjustPanLimits(vw, vh);
}


void Canvas::AdjustPanLimits(int vw, int vh)
{
    int w = zoomRect.right - zoomRect.left;
    int h = zoomRect.bottom - zoomRect.top;

    if (w > vw)
    {
        if (zoomRect.left > vw - 100)
        {
            zoomRect.left = vw - 100;
            zoomRect.right = zoomRect.left + w;
        }
        else if (zoomRect.right < 100)
        {
            zoomRect.right = 100;
            zoomRect.left = zoomRect.right - w;
        }
    }

    if (h > vh)
    {
        if (zoomRect.top > vh - 100)
        {
            zoomRect.top = vh - 100;
            zoomRect.bottom = zoomRect.top + h;
        }
        else if (zoomRect.bottom < 100)
        {
            zoomRect.bottom = 100;
            zoomRect.top = zoomRect.bottom - h;
        }
    }
}