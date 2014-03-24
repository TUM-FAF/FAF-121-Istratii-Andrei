#pragma once

#include <windows.h>
#include <list>
#include "drawable.h"
#include "mouse.h"




class Canvas : public IMouseListener
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

    virtual void OnLeftMouseButtonDown(int x, int y);
    virtual void OnLeftMouseButtonUp(int x, int y);
    virtual void OnRightMouseButtonDown(int x, int y);
    virtual void OnRightMouseButtonUp(int x, int y);
    virtual void OnMouseMove(int x, int y);

    void Clear();
    void Update();

private:
    HDC hDC;
    HBITMAP hBMP;
    HBITMAP hOldBMP;

    RECT zoomRect;
    float zoomFactor;

    int width;
    int height;

    Drawable* tempObject;

    bool isDrawing;

    std::list<Drawable*> objects;


    void ViewToCanvasCoords(POINT* pt);

    void AdjustPanLimits(int vw, int vh);

    float NextZoomStep();
    float PrevZoomStep();

    static float const zoomSteps[25];
};
