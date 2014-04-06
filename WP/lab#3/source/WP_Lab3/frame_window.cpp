
#include "frame_window.h"
#include "shared_headers.h"


void FrameWindow::OnCreate()
{
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

    RECT rct;
    GetClientRect(hWnd, &rct);

    toolbar.Create(WS_CHILD|WS_VISIBLE|CCS_VERT, hWnd, hInstance);

    workspace.CreateEx(WS_EX_CLIENTEDGE,
                    L"workspace",
                    L"",
                    WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
                    120, 20,
                    rct.right - rct.left - 140, rct.bottom - rct.top - 40,
                    hWnd, NULL,
                    hInstance);

    fillColorPicker.Create(40, 200, 20, 20,
        hWnd, (HMENU)IDM_COLORPICKER_FILL,
        hInstance, workspace.GetFillColor());

    strokeColorPicker.Create(40, 230, 20, 20,
        hWnd, (HMENU)IDM_COLORPICKER_STROKE,
        hInstance, workspace.GetStrokeColor());


    for (int i=0; i<16; ++i)
    {
        customColors[i] = RGB(255, 255, 255);
    }
}



void FrameWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
    case IDM_TBITEM1:
        std::cout << "TBITEM111\n";
        break;

    case IDM_COLORPICKER_FILL:
        if (HIWORD(wParam) == STN_CLICKED)
        {
            CHOOSECOLOR chc = {0};
            chc.lStructSize = sizeof(CHOOSECOLOR);
            chc.hwndOwner = hWnd;
            chc.lpCustColors = customColors;
            chc.rgbResult = workspace.GetFillColor();
            chc.Flags = CC_RGBINIT | CC_FULLOPEN;
            if (ChooseColor(&chc) == TRUE)
            {
                workspace.SetFillColor(chc.rgbResult);
                InvalidateRect(fillColorPicker.GetHWND(), NULL, FALSE);
            }
        }
        break;

    case IDM_COLORPICKER_STROKE:
        if (HIWORD(wParam) == STN_CLICKED)
        {
            CHOOSECOLOR chc = {0};
            chc.lStructSize = sizeof(CHOOSECOLOR);
            chc.hwndOwner = hWnd;
            chc.lpCustColors = customColors;
            chc.rgbResult = workspace.GetStrokeColor();
            chc.Flags = CC_RGBINIT | CC_FULLOPEN;
            if (ChooseColor(&chc) == TRUE)
            {
                workspace.SetStrokeColor(chc.rgbResult);
                InvalidateRect(strokeColorPicker.GetHWND(), NULL, FALSE);
            }
        }
        break;

    default:
        break;
    }
}


void FrameWindow::OnSize(int width, int height, WPARAM wParam)
{
    SetWindowPos(workspace.GetHWND(), HWND_TOP, 0, 0, width - 140, height - 40, SWP_NOMOVE|SWP_NOREPOSITION);
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
    case WM_MOUSEWHEEL:
        SendMessage(workspace.GetHWND(), message, wParam, lParam);
        break;

    case WM_CTLCOLORSTATIC:
        {
            HWND hColorPicker = (HWND)lParam;

            if (hColorPicker == fillColorPicker.GetHWND())
            {
                HBRUSH hbr = fillColorPicker.UpdateBrush(workspace.GetFillColor());
                return (LRESULT)hbr;
            }

            if (hColorPicker == strokeColorPicker.GetHWND())
            {
                HBRUSH hbr = strokeColorPicker.UpdateBrush(workspace.GetStrokeColor());
                return (LRESULT)hbr;
            }
        }
        break;

    default:
        return Window::WndProc(hWnd_, message, wParam, lParam);
    }

    return 0;
}