
#include "viewport_window.h"
#include "shared_headers.h"


ViewportWindow::ViewportWindow()
{

}


void ViewportWindow::FillWndClassEx(WNDCLASSEX & wcex)
{
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
}


void ViewportWindow::OnCreate()
{
    RECT rct;
    GetClientRect(hWnd, &rct);

    HDC hDC = GetDC(hWnd);

    backbuffer.Init(hDC, rct.right - rct.left, rct.bottom - rct.top);

    ReleaseDC(hWnd, hDC);

    repaintTimer = SetTimer(hWnd, ID_REPAINT_TIMER, 16, NULL);
}


void ViewportWindow::OnPaint(HDC hDC)
{
    backbuffer.FillWithColor(RGB(100, 100, 100));
    backbuffer.Present(hDC);
}


void ViewportWindow::OnSize(int width, int height, WPARAM wParam)
{
    backbuffer.Resize(width, height);
}


void ViewportWindow::OnDestroy()
{
    KillTimer(hWnd, repaintTimer);
}


LRESULT ViewportWindow::WndProc(HWND hWnd_, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ERASEBKGND:
        return 1;
        break;


    case WM_LBUTTONDOWN:
        SetCapture(hWnd);
        //mouse.LeftButtonDown();
        break;


    case WM_LBUTTONUP:
        ReleaseCapture();
        //mouse.LeftButtonUp();
        break;


    case WM_RBUTTONDOWN:
        SetCapture(hWnd);
        //mouse.RightButtonDown();
        break;


    case WM_RBUTTONUP:
        ReleaseCapture();
        //mouse.RightButtonUp();
        break;



    case WM_TIMER:
        InvalidateRect(hWnd, NULL, FALSE);
        break;



    case WM_MOUSEMOVE:
        {
            SetFocus(hWnd);
            POINT m;
            m.x = GET_X_LPARAM(lParam);
            m.y = GET_Y_LPARAM(lParam);

            //mouse.Update(m.x, m.y);
            //mouse.Move();

            Ellipse(backbuffer.GetDC(), m.x, m.y, m.x+10, m.y+10);
        }
        break;


    default:
        return Window::WndProc(hWnd_, message, wParam, lParam);
    }

    return 0;
}