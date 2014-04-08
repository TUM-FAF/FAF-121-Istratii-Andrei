#pragma once


#include <windows.h>
#include "window.h"
#include "viewport_window.h"




class FrameWindow : public Window
{
private:
    ViewportWindow viewport;

    HFONT hFont;

    virtual void OnCreate();
    virtual void OnSize(int width, int height, WPARAM wParam);
    virtual void OnCommand(WPARAM wParam, LPARAM lParam);
    virtual void OnDestroy();

    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void FillWndClassEx(WNDCLASSEX & wcex);
};