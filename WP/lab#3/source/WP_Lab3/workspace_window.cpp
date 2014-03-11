
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


void WorkspaceWindow::OnPaint(HDC hDC)
{
    HDC hMemDC;
    HBITMAP hBMP;
    HBITMAP hOldBMP;

    hMemDC = CreateCompatibleDC(hDC);
    hBMP = LoadBitmap((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(ID_DUKE_BMP));
    hOldBMP = (HBITMAP)SelectObject(hMemDC, hBMP);

    RECT clippingRect;

    SetStretchBltMode(hDC, HALFTONE);
    StretchBlt(hDC,
        30,
        30,
        130,
        130,
        hMemDC,
        0,
        0,
        600,
        600,
        SRCCOPY);


    if (hOldBMP) { SelectObject(hMemDC, hOldBMP); }
    if (hBMP) { DeleteObject(hBMP); }
    if (hDC) { DeleteDC(hMemDC); }

}


LRESULT WorkspaceWindow::WndProc(HWND hWnd_, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        hWnd = hWnd_;
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


    case WM_DESTROY:
        break;


    default:
        return DefWindowProc(hWnd_, message, wParam, lParam);
    }

    return 0;
}