#pragma once


#include <windows.h>
#include "window.h"
#include "backbuffer.h"
//#include "mouse.h"



class ViewportWindow : public Window
{
public:
    ViewportWindow();
    void InitCanvas(int w, int h);

private:
    UINT_PTR repaintTimer;
    Backbuffer backbuffer;
    Backbuffer canvas;

    //Mouse mouse;

    virtual void OnCreate();
    virtual void OnPaint(HDC hDC);
    virtual void OnSize(int width, int height, WPARAM wParam);
    virtual void OnDestroy();


    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void FillWndClassEx(WNDCLASSEX & wcex);
};