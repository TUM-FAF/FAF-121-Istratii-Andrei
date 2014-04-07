
#include "frame_window.h"
#include "shared_headers.h"


void FrameWindow::OnCreate()
{
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

    RECT rct;
    GetClientRect(hWnd, &rct);

    //toolbar.Create(WS_CHILD|WS_VISIBLE|CCS_VERT, hWnd, hInstance);

    int width = rct.right - rct.left;
    int height = rct.bottom - rct.top;

    workspace.CreateEx(WS_EX_STATICEDGE,
                    L"workspace",
                    L"",
                    WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
                    120, 20,
                    width - 140, height - 40,
                    hWnd, NULL,
                    hInstance);


    CreateWindowEx(0, L"STATIC", L"Tools:",
        WS_CHILD|WS_VISIBLE,
        10, 20,
        50, 20,
        hWnd, NULL,
        hInstance, 0);

    CreateWindowEx(0, L"BUTTON", L"Select",
        WS_CHILD|WS_VISIBLE|WS_GROUP|BS_AUTORADIOBUTTON|BS_PUSHLIKE|BS_ICON,
        20, 40,
        32, 32,
        hWnd, (HMENU)IDM_RADIO_SELECT,
        hInstance, 0);

    SendMessage(GetDlgItem(hWnd, IDM_RADIO_SELECT), BM_SETCHECK, BST_CHECKED, NULL);

    CreateWindowEx(0, L"BUTTON", L"Pencil",
        WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|BS_PUSHLIKE|BS_ICON,
        55, 40,
        32, 32,
        hWnd, (HMENU)IDM_RADIO_PENCIL,
        hInstance, 0);

    CreateWindowEx(0, L"BUTTON", L"Ellipse",
        WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|BS_PUSHLIKE|BS_ICON,
        20, 75,
        32, 32,
        hWnd, (HMENU)IDM_RADIO_ELLIPSE,
        hInstance, 0);

    CreateWindowEx(0, L"BUTTON", L"Rectangle",
        WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|BS_PUSHLIKE|BS_ICON,
        55, 75,
        32, 32,
        hWnd, (HMENU)IDM_RADIO_RECTANGLE,
        hInstance, 0);

    CreateWindowEx(0, L"BUTTON", L"ZoomIn",
        WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|BS_PUSHLIKE|BS_ICON,
        20, 110,
        32, 32,
        hWnd, (HMENU)IDM_RADIO_ZOOMIN,
        hInstance, 0);

    CreateWindowEx(0, L"BUTTON", L"ZoomOut",
        WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|BS_PUSHLIKE|BS_ICON,
        55, 110,
        32, 32,
        hWnd, (HMENU)IDM_RADIO_ZOOMOUT,
        hInstance, 0);

    HICON el;
    el = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POINTER_ICON));
    SendMessage(GetDlgItem(hWnd, IDM_RADIO_SELECT), BM_SETIMAGE, IMAGE_ICON, (LPARAM)el);

    el = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PENCIL_ICON));
    SendMessage(GetDlgItem(hWnd, IDM_RADIO_PENCIL), BM_SETIMAGE, IMAGE_ICON, (LPARAM)el);

    el = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ELLIPSE_ICON));
    SendMessage(GetDlgItem(hWnd, IDM_RADIO_ELLIPSE), BM_SETIMAGE, IMAGE_ICON, (LPARAM)el);

    el = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RECTANGLE_ICON));
    SendMessage(GetDlgItem(hWnd, IDM_RADIO_RECTANGLE), BM_SETIMAGE, IMAGE_ICON, (LPARAM)el);

    el = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ZOOMIN_ICON));
    SendMessage(GetDlgItem(hWnd, IDM_RADIO_ZOOMIN), BM_SETIMAGE, IMAGE_ICON, (LPARAM)el);

    el = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ZOOMOUT_ICON));
    SendMessage(GetDlgItem(hWnd, IDM_RADIO_ZOOMOUT), BM_SETIMAGE, IMAGE_ICON, (LPARAM)el);


    // fill/stroke options
    fillColorPicker.Create(80, 200, 20, 20,
        hWnd, (HMENU)IDM_COLORPICKER_FILL,
        hInstance, workspace.drawingOptions.fillColor);

    strokeColorPicker.Create(80, 240, 20, 20,
        hWnd, (HMENU)IDM_COLORPICKER_STROKE,
        hInstance, workspace.drawingOptions.strokeColor);


    CreateWindowEx(0, L"BUTTON", L"Fill",
        WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
        10, 200,
        50, 20,
        hWnd, (HMENU)IDM_CHKBX_FILL,
        hInstance, 0);

    if (!workspace.drawingOptions.noFill)
    {
        Button_SetCheck(GetDlgItem(hWnd, IDM_CHKBX_FILL), BST_CHECKED);
    }

    CreateWindowEx(0, L"BUTTON", L"Stroke",
        WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
        10, 240,
        70, 20,
        hWnd, (HMENU)IDM_CHKBX_STROKE,
        hInstance, 0);

    if (!workspace.drawingOptions.noStroke)
    {
        Button_SetCheck(GetDlgItem(hWnd, IDM_CHKBX_STROKE), BST_CHECKED);
    }

    CreateWindowEx(0, L"STATIC", L"Width:",
        WS_CHILD|WS_VISIBLE,
        28, 265,
        50, 20,
        hWnd, NULL,
        hInstance, 0);

    CreateWindowEx(WS_EX_STATICEDGE, L"EDIT", L"1",
        WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER,
        80, 265,
        20, 20,
        hWnd, NULL,
        hInstance, 0);


    for (int i=0; i<16; ++i)
    {
        customColors[i] = RGB(255, 255, 255);
    }


    hFont = CreateFont(17, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Verdana");
    SetFontForChildren(hWnd, hFont);
}



void FrameWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch (HIWORD(wParam))
    {
    case BN_CLICKED: // also includes STN_CLICKED:
        switch (LOWORD(wParam))
        {
        case IDM_COLORPICKER_FILL:
            {
                CHOOSECOLOR chc = {0};
                chc.lStructSize = sizeof(CHOOSECOLOR);
                chc.hwndOwner = hWnd;
                chc.lpCustColors = customColors;
                chc.rgbResult = workspace.drawingOptions.fillColor;
                chc.Flags = CC_RGBINIT | CC_FULLOPEN;
                if (ChooseColor(&chc) == TRUE)
                {
                    workspace.drawingOptions.fillColor = chc.rgbResult;
                    InvalidateRect(fillColorPicker.GetHWND(), NULL, FALSE);
                }
            }
            break;

        case IDM_COLORPICKER_STROKE:
            {
                CHOOSECOLOR chc = {0};
                chc.lStructSize = sizeof(CHOOSECOLOR);
                chc.hwndOwner = hWnd;
                chc.lpCustColors = customColors;
                chc.rgbResult = workspace.drawingOptions.strokeColor;
                chc.Flags = CC_RGBINIT | CC_FULLOPEN;
                if (ChooseColor(&chc) == TRUE)
                {
                    workspace.drawingOptions.strokeColor = chc.rgbResult;
                    InvalidateRect(strokeColorPicker.GetHWND(), NULL, FALSE);
                }
            }
            break;

        case IDM_RADIO_SELECT:
            workspace.drawingOptions.tool = SELECT;
            break;

        case IDM_RADIO_PENCIL:
            workspace.drawingOptions.tool = PENCIL;
            break;

        case IDM_RADIO_ELLIPSE:
            workspace.drawingOptions.tool = ELLIPSE;
            break;

        case IDM_RADIO_RECTANGLE:
            workspace.drawingOptions.tool = RECTANGLE;
            break;

        case IDM_RADIO_ZOOMIN:
            workspace.drawingOptions.tool = ZOOMIN;
            break;

        case IDM_RADIO_ZOOMOUT:
            workspace.drawingOptions.tool = ZOOMOUT;
            break;


        case IDM_CHKBX_FILL:
            if (Button_GetCheck(GetDlgItem(hWnd, IDM_CHKBX_FILL)) == BST_CHECKED)
            {
                workspace.drawingOptions.noFill = false;
            }
            else
            {
                workspace.drawingOptions.noFill = true;
            }
            break;

        case IDM_CHKBX_STROKE:
            if (Button_GetCheck(GetDlgItem(hWnd, IDM_CHKBX_STROKE)) == BST_CHECKED)
            {
                workspace.drawingOptions.noStroke = false;
            }
            else
            {
                workspace.drawingOptions.noStroke = true;
            }
            break;

        default:
            break;
        }

    default:
        break;
    }
}



void FrameWindow::OnSize(int width, int height, WPARAM wParam)
{
    SetWindowPos(workspace.GetHWND(), HWND_TOP, 0, 0, width - 140, height - 40, SWP_NOMOVE|SWP_NOREPOSITION);
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
        SendMessage(workspace.GetHWND(), message, wParam, lParam);
        break;

    case WM_CTLCOLORSTATIC:
        {
            HWND hColorPicker = (HWND)lParam;

            if (hColorPicker == fillColorPicker.GetHWND())
            {
                HBRUSH hbr = fillColorPicker.UpdateBrush(workspace.drawingOptions.fillColor);
                return (LRESULT)hbr;
            }

            if (hColorPicker == strokeColorPicker.GetHWND())
            {
                HBRUSH hbr = strokeColorPicker.UpdateBrush(workspace.drawingOptions.strokeColor);
                return (LRESULT)hbr;
            }
        }
        break;

    default:
        return Window::WndProc(hWnd_, message, wParam, lParam);
    }

    return 0;
}