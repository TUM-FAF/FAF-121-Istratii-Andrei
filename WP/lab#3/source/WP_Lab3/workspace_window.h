#pragma once


#include <windows.h>
#include "window.h"
#include "canvas.h"




class WorkspaceWindow : public Window
{
public:
    WorkspaceWindow();
    void InitCanvas(int w, int h);

private:
    Canvas* canvas;
    RECT bltRect;

    virtual void OnPaint(HDC hDC);

    void AdjustBltRect();

    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void FillWndClassEx(WNDCLASSEX & wcex);
};