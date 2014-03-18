
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


void Canvas::Zoom(float zf, int mx, int my, int vw, int vh)
{
    int w = zoomRect.right - zoomRect.left;
    int h = zoomRect.bottom - zoomRect.top;

    std::cout << zf << std::endl;
    std::cout << mx << " :: " << my << std::endl;
    std::cout << zoomRect.left << ", " << zoomRect.top << ", " << zoomRect.right << ", " << zoomRect.bottom << std::endl << std::endl;

    int newWidth = width*zf;
    int newHeight = height*zf;

    float xPart = (float)(zoomRect.left - mx)/(float)w;
    float yPart = (float)(zoomRect.top - my)/(float)h;

    zoomRect.left = xPart * newWidth + mx;
    zoomRect.top = yPart * newHeight + my;

    zoomRect.right = zoomRect.left + width*zf;
    zoomRect.bottom = zoomRect.top + height*zf;

    AdjustPanLimits(vw, vh);
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