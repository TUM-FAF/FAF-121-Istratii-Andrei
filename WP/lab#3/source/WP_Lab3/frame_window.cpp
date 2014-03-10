
#include "frame_window.h"

#include <iostream>


void FrameWindow::OnCreate()
{
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

    RECT rct;
    GetClientRect(hWnd, &rct);

    workspace.CreateEx(WS_EX_CLIENTEDGE,
                    L"workspace",
                    L"",
                    WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
                    120, 20,
                    rct.right - rct.left - 140, rct.bottom - rct.top - 40,
                    hWnd, NULL,
                    hInstance);
}



void FrameWindow::FillWndClassEx(WNDCLASSEX & wcex)
{
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
}



LRESULT FrameWindow::WndProc(HWND hWnd_, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            hWnd = hWnd_;
            OnCreate();
        }
        break;

    case WM_SIZE:
        {
            int clientWidth = LOWORD(lParam);
            int clientHeight = HIWORD(lParam);

            SetWindowPos(workspace.GetHWND(), HWND_TOP, 0, 0, clientWidth - 140, clientHeight - 40, SWP_NOMOVE|SWP_NOREPOSITION);
        }
        break;


    case WM_MOUSEWHEEL:
        SendMessage(workspace.GetHWND(), message, wParam, lParam);
        break;


    case WM_CLOSE:
        DestroyWindow(hWnd_);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd_, message, wParam, lParam);
    }

    return 0;
}