
#include "workspace_window.h"



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
    // iterate through objects and paint them
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