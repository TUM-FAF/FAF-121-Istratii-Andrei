
#include "workspace_window.h"
#include "shared_headers.h"


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

    canvas = new Canvas(&drawingOptions);
    canvas->Init(hDC, 1024, 1024);
    mouse.RegisterListener(canvas);

    ReleaseDC(hWnd, hDC);
}


void WorkspaceWindow::OnPaint(HDC hDC)
{
    backbuffer.FillWithColor(RGB(100, 100, 100));

    canvas->Update();

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

    HBRUSH hOldBrush = (HBRUSH)SelectObject(backbuffer.GetDC(), GetStockBrush(NULL_BRUSH));

    Rectangle(backbuffer.GetDC(), zoom.left-1, zoom.top-1, zoom.right+1, zoom.bottom+1);

    SelectObject(backbuffer.GetDC(), hOldBrush);

    backbuffer.Present(hDC);
}


void WorkspaceWindow::OnSize(int width, int height, WPARAM wParam)
{
    backbuffer.Resize(width, height);
}


void WorkspaceWindow::OnDestroy()
{
    if (canvas) { delete canvas; }
}


LRESULT WorkspaceWindow::WndProc(HWND hWnd_, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ERASEBKGND:
        return 1;
        break;


    case WM_MOUSEWHEEL:
        {
            RECT rct;
            GetClientRect(hWnd, &rct);

            canvas->Zoom(mouse.X(), mouse.Y(), rct.right - rct.left, rct.bottom - rct.top);

            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;


    case WM_LBUTTONDOWN:
        SetCapture(hWnd);
        mouse.LeftButtonDown();
        {
            RECT rct;
            GetClientRect(hWnd, &rct);

            if (drawingOptions.tool == ZOOMIN)
            {
                canvas->StepZoomIn(mouse.X(), mouse.Y(), rct.right - rct.left, rct.bottom - rct.top);
                InvalidateRect(hWnd, NULL, FALSE);
            }
            if (drawingOptions.tool == ZOOMOUT)
            {
                canvas->StepZoomOut(mouse.X(), mouse.Y(), rct.right - rct.left, rct.bottom - rct.top);
                InvalidateRect(hWnd, NULL, FALSE);
            }
        }
        break;


    case WM_LBUTTONUP:
        ReleaseCapture();
        mouse.LeftButtonUp();
        break;


    case WM_RBUTTONDOWN:
        SetCapture(hWnd);
        mouse.RightButtonDown();
        break;


    case WM_RBUTTONUP:
        ReleaseCapture();
        mouse.RightButtonUp();
        break;


    case WM_KEYDOWN:
        if (wParam == VK_SHIFT)
        {
            drawingOptions.shiftConstraint = true;
        }
        break;

    case WM_KEYUP:
        if (wParam == VK_SHIFT)
        {
            drawingOptions.shiftConstraint = false;
        }
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
                GetClientRect(hWnd, &rct);
                canvas->Pan(mouse.DX(), mouse.DY(), rct.right - rct.left, rct.bottom - rct.top);
            }
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;


    default:
        return Window::WndProc(hWnd_, message, wParam, lParam);
    }

    return 0;
}