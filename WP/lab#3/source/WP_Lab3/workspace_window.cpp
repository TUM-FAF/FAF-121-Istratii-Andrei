
#include "workspace_window.h"
#include "resource.h"
#include "canvas.h"
#include <windowsx.h>

#include <iostream>


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

    canvas = new Canvas;

    canvas->Init(hDC, 1024, 1024);
    zoomFactor = 1.0;

    mouse.RegisterListener(&intermediateDrawer);

    ReleaseDC(hWnd, hDC);
}


void WorkspaceWindow::OnPaint(HDC hDC)
{
    backbuffer.FillWithColor(RGB(200,255,200));

    RECT zoom = canvas->GetZoomRect();

    SetStretchBltMode(backbuffer.GetDC(), HALFTONE);
    StretchBlt(backbuffer.GetDC(),
        zoom.left,
        zoom.top,
        zoom.right - zoom.left,
        zoom.bottom - zoom.top,
        canvas->GetDC(),
        0,
        0,
        canvas->GetWidth(),
        canvas->GetHeight(),
        SRCCOPY);

    intermediateDrawer.Render(backbuffer.GetDC());

    backbuffer.Present(hDC);
}


void WorkspaceWindow::OnSize(int w, int h)
{
    backbuffer.Resize(w, h);
}


void WorkspaceWindow::OnDestroy()
{
    if (canvas) { delete canvas; }
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
        {
            zoomFactor += (GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? 0.05 : -0.05;

            RECT rct;
            GetClientRect(hWnd, &rct);

            canvas->Zoom(zoomFactor, mouse.X(), mouse.Y(), rct.right - rct.left, rct.bottom - rct.top);

            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;


    case WM_LBUTTONDOWN:
        SetCapture(hWnd);
        mouse.LeftButtonDown();
        break;


    case WM_LBUTTONUP:
        ReleaseCapture();
        mouse.LeftButtonUp();
        break;


    case WM_MOUSEMOVE:
        {
            SetFocus(hWnd);
            POINT m;
            m.x = GET_X_LPARAM(lParam);
            m.y = GET_Y_LPARAM(lParam);

            mouse.Update(m.x, m.y);
            mouse.Move();

            if (wParam == MK_RBUTTON)
            {
                RECT rct;
                GetClientRect(hWnd_, &rct);
                canvas->Pan(mouse.DX(), mouse.DY(), rct.right - rct.left, rct.bottom - rct.top);
            }
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;


    case WM_KEYDOWN:
        if (wParam == VK_UP)
        {
            zoomFactor += 0.05;
            canvas->Zoom(zoomFactor, mouse.X(), mouse.Y(), 0, 0);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        if (wParam == VK_DOWN)
        {
            zoomFactor -= 0.05;
            canvas->Zoom(zoomFactor, mouse.X(), mouse.Y(), 0, 0);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;


    case WM_DESTROY:
        OnDestroy();
        break;


    default:
        return DefWindowProc(hWnd_, message, wParam, lParam);
    }

    return 0;
}