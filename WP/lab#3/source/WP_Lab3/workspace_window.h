#pragma once


#include <windows.h>
#include "window.h"
#include "backbuffer.h"
#include "mouse.h"
#include "drawing_options.h"


class Canvas;


class WorkspaceWindow : public Window
{
public:
    WorkspaceWindow();
    void InitCanvas(int w, int h);

    DrawingOptions drawingOptions;

private:
    Canvas* canvas;
    
    Backbuffer backbuffer;

    Mouse mouse;

    float zoomFactor;

    virtual void OnCreate();
    virtual void OnPaint(HDC hDC);
    virtual void OnSize(int width, int height, WPARAM wParam);
    virtual void OnDestroy();


    void AdjustBltRect();

    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void FillWndClassEx(WNDCLASSEX & wcex);
};