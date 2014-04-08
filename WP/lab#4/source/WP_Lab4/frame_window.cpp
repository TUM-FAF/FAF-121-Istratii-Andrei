
#include "frame_window.h"
#include "shared_headers.h"


void FrameWindow::OnCreate()
{
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

    RECT rct;
    GetClientRect(hWnd, &rct);


    int width = rct.right - rct.left;
    int height = rct.bottom - rct.top;

    viewport.CreateEx(WS_EX_STATICEDGE,
                    L"viewport",
                    L"",
                    WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
                    120, 20,
                    width - 140, height - 40,
                    hWnd, NULL,
                    hInstance);



    hFont = CreateFont(17, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Verdana");
    SetFontForChildren(hWnd, hFont);
}



void FrameWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch (HIWORD(wParam))
    {
    case BN_CLICKED: // also includes STN_CLICKED:

        break;

    default:
        break;
    }
}



void FrameWindow::OnSize(int width, int height, WPARAM wParam)
{
    SetWindowPos(viewport.GetHWND(), HWND_TOP, 0, 0, width - 140, height - 40, SWP_NOMOVE|SWP_NOREPOSITION);
}


void FrameWindow::OnDestroy()
{
    DeleteFont(hFont);
    PostQuitMessage(0);
}



void FrameWindow::FillWndClassEx(WNDCLASSEX & wcex)
{
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
}



LRESULT FrameWindow::WndProc(HWND hWnd_, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_MOUSEWHEEL:
        SendMessage(viewport.GetHWND(), message, wParam, lParam);
        break;


    default:
        return Window::WndProc(hWnd_, message, wParam, lParam);
    }

    return 0;
}
