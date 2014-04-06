
#pragma once

#include <windows.h>
#include "window.h"

class Colorpicker : public Window
{
public:
    Colorpicker()
    {
        hBgBr = NULL;
    }

    virtual ~Colorpicker()
    {
        if (hBgBr) { DeleteObject(hBgBr); }
    }

    void Create(int x, int y,
        int nWidth, int nHeight,
        HWND hParent, HMENU hMenu,
        HINSTANCE hInst, COLORREF color)
    {
        Window::CreateEx(WS_EX_CLIENTEDGE, L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_NOTIFY,
            x, y, nWidth, nHeight,
            hParent, hMenu, hInst);

        hBgBr = CreateSolidBrush(color);
    }

    HBRUSH UpdateBrush(COLORREF color)
    {
        DeleteObject(hBgBr);
        hBgBr = CreateSolidBrush(color);
        return hBgBr;
    }

private:
    HBRUSH hBgBr;
};