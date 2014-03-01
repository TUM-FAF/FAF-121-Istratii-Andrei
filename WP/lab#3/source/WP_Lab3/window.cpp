
#include "window.h"



bool Window::CreateEx(DWORD dwExStyle,
                      LPCTSTR lpszClass, LPCTSTR lpszName,
                      DWORD dwStyle,
                      int x, int y,
                      int nWidth, int nHeight,
                      HWND hParent, HMENU hMenu, HINSTANCE hInstance)
{
    if (!RegisterClass(lpszClass, hInstance))
    {
        return false;
    }

    MDICREATESTRUCT mdic;
    memset(&mdic, 0, sizeof(mdic));
    mdic.lParam = (LPARAM)this;

    hWnd = CreateWindowEx(dwExStyle, lpszClass, lpszName, dwStyle
                            x, y, nWidth, nHeight,
                            hParent, hMenu, hInstance, &mdic);

    return (hWnd != NULL);
}


bool Window::RegisterClass(LPCTSTR lpszClass, HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    if (!GetClassInfoEx(hInstance, lpszClass, &wcex))
    {
        FillWndClassEx(wcex);

        wcex.hInstance = hInstance;
        wcex.lpszClassName = lpszClass;

        if (!RegisterClassEx(&wcex)) { return false; }
    }

    return true;
}


void Window::FillWndClassEx(WNDCLASSEX & wcex)
{
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = 0;
    wcex.lpfnWndProc    = WindowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = NULL;
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = NULL;
    wcex.hIconSm        = NULL;
}



LRESULT Window::WndProc(HWND hWnd_, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_KEYDOWN:
        OnKeyDown(wParam, lParam);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;

            BeginPaint(hWnd, &ps);
            OnDraw(ps.hdc);
            EndPaint(hWnd, &ps);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT CALLBACK Window::WindowProc(HWND hWnd_, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window* pWin = NULL;

    if (message == WM_NCCREATE)
    {
        MDICREATESTRUCT* pMDIC = (MDICREATESTRUCT*) ((LPCREATESTRUCT) lParam)->lpCreateParams;
        pWin = (Window*) (pMDIC->lParam);

        SetWindowLong(hWnd_, GWL_USERDATA, (LONG)pWin);
    } 
    else
    {
        pWin = (Window*)GetWindowLong(hWnd_, GWL_USERDATA);
    }

    if (pWin)
    {
        return pWin->WndProc(hWnd_, message, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hWnd_, message, wParam, lParam);
    }
}


