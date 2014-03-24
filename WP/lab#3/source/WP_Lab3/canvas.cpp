
#include "canvas.h"
#include "shared_headers.h"


float const Canvas::zoomSteps[] = {
    0.01f, 0.015f, 0.02f, 0.03f, 0.04f, 0.05f,
    0.0625f, 0.0833f, 0.125f, 0.1667f, 0.25f, 0.3333f, 0.5f, 0.6667f, 1.0f,
    2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 12.0f, 16.0f, 32.0f
};


Canvas::Canvas()
{
    hDC = NULL;
    hBMP = NULL;
    hOldBMP = NULL;

    width = 0;
    height = 0;

    zoomFactor = 1.0f;
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


void Canvas::StepZoomIn(int mx, int my, int vw, int vh)
{
    NextZoomStep();
    Zoom(mx, my, vw, vh);
}


void Canvas::StepZoomOut(int mx, int my, int vw, int vh)
{
    PrevZoomStep();
    Zoom(mx, my, vw, vh);
}


void Canvas::Zoom(int mx, int my, int vw, int vh)
{
    int w = zoomRect.right - zoomRect.left;
    int h = zoomRect.bottom - zoomRect.top;

    std::cout << zoomFactor << std::endl;


    int newWidth = (int)(width * zoomFactor);
    int newHeight = (int)(height * zoomFactor);

    float xPart = (float)(zoomRect.left - mx)/(float)w;
    float yPart = (float)(zoomRect.top - my)/(float)h;

    zoomRect.left = (int)(xPart * newWidth) + mx;
    zoomRect.top = (int)(yPart * newHeight) + my;

    zoomRect.right = zoomRect.left + newWidth;
    zoomRect.bottom = zoomRect.top + newHeight;

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


float Canvas::NextZoomStep()
{
    char i = 1;

    while (i != 24 && zoomFactor > zoomSteps[i-1])
    {
        ++i;
    }

    zoomFactor = zoomSteps[i];

    return zoomFactor;
}


float Canvas::PrevZoomStep()
{
    char i = 23;

    while (i != 0 && zoomFactor < zoomSteps[i+1])
    {
        --i;
    }

    zoomFactor = zoomSteps[i];

    return zoomFactor;
}
