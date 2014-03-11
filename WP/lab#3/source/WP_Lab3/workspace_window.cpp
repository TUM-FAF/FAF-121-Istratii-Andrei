
#include "workspace_window.h"
#include "resource.h"



WorkspaceWindow::WorkspaceWindow()
{
    canvas = NULL;
}


void WorkspaceWindow::FillWndClassEx(WNDCLASSEX & wcex)
{
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
}


void WorkspaceWindow::OnCreate()
{
    RECT rct;
    GetClientRect(hWnd, &rct);

    HDC hDC = GetDC(hWnd);

    backbuffer.Init(hDC, rct.right - rct.left, rct.bottom - rct.top);

    ReleaseDC(hWnd, hDC);
}


void WorkspaceWindow::OnPaint(HDC hDC)
{
    HDC hMemDC;
    HBITMAP hBMP;
    HBITMAP hOldBMP;

    hMemDC = CreateCompatibleDC(backbuffer.GetDC());
    hBMP = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(ID_DUKE_BMP));
    hOldBMP = (HBITMAP)SelectObject(hMemDC, hBMP);


    backbuffer.FillWithColor(RGB(200,255,200));


    SetStretchBltMode(backbuffer.GetDC(), HALFTONE);
    StretchBlt(backbuffer.GetDC(),
        -50,
        -50,
        1000,
        1000,
        hMemDC,
        0,
        0,
        600,
        600,
        SRCCOPY);

    backbuffer.Present(hDC);

    if (hOldBMP) { SelectObject(hMemDC, hOldBMP); }
    if (hBMP) { DeleteObject(hBMP); }
    if (hDC) { DeleteDC(hMemDC); }

}


void WorkspaceWindow::OnSize(int w, int h)
{
    backbuffer.Resize(w, h);
}


LRESULT WorkspaceWindow::WndProc(HWND hWnd_, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        hWnd = hWnd_;
        OnCreate();
        break;


    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            OnPaint(ps.hdc);
            EndPaint(hWnd, &ps);
        }
        break;


    case WM_ERASEBKGND:
        return 1;
        break;


    case WM_SIZE:
        OnSize(LOWORD(lParam), HIWORD(lParam));
        break;


    case WM_MOUSEWHEEL:

        break;


    case WM_MOUSEMOVE:
        if (wParam == MK_LBUTTON)
        {
            
        }
        break;


    case WM_DESTROY:
        break;


    default:
        return DefWindowProc(hWnd_, message, wParam, lParam);
    }

    return 0;
}