#pragma once


#include <windows.h>
#include "window.h"
#include "canvas.h"




class WorkspaceWindow : public Window
{
public:
    WorkspaceWindow();

private:
    Canvas* canvas;

    virtual void OnPaint(HDC hDC);

    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void FillWndClassEx(WNDCLASSEX & wcex);
};