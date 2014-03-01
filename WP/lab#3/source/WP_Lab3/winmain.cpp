#pragma comment( lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")



#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>

#include <io.h>
#include <cstdio>
#include <fcntl.h>

#include "resource.h"

using namespace std;


map<string, string> g_options;
LPCWSTR g_mainWinClassName = L"lab3";
LPCWSTR g_workspaceWinClassName = L"workspace";


LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WorkspaceProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegisterMainWindow(HINSTANCE, LPCWSTR, WNDPROC);
bool LoadConfigFile(char const * filePath);


void CreateDeveloperConsole();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{


    HWND hWnd;
    MSG msg;

    INITCOMMONCONTROLSEX ctls;
    ctls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    ctls.dwICC = ICC_BAR_CLASSES;

    InitCommonControlsEx(&ctls);

    CreateDeveloperConsole();


    if (!LoadConfigFile("settings.cfg"))
    {
        MessageBox(NULL, L"Could not load config file", L"Error", MB_OK);
        return 0;
    }


    if (!RegisterMainWindow(hInstance, g_mainWinClassName, MainWinProc))
    {
        MessageBox(NULL, L"Could not register main window class!", L"Error!", MB_OK|MB_ICONERROR);
        return 0;
    }

    if (!RegisterMainWindow(hInstance, g_workspaceWinClassName, WorkspaceProc))
    {
        MessageBox(NULL, L"Could not register workspace window class!", L"Error!", MB_OK|MB_ICONERROR);
        return 0;
    }

    RECT rct;

    rct.left = 0;
    rct.top = 0;
    rct.right = stoi(g_options["window_width"]);
    rct.bottom = stoi(g_options["window_height"]);

    AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, TRUE, WS_EX_OVERLAPPEDWINDOW);

    hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                            g_mainWinClassName,
                            L"WP Lab 3",
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, 0,
                            rct.right - rct.left, rct.bottom - rct.top,
                            NULL, NULL,
                            hInstance,
                            NULL);


    if (!hWnd)
    {
        MessageBox(NULL, L"Could not create the window!", L"Error!", MB_OK|MB_ICONERROR);
        return 0;
    }


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    while (GetMessage(&msg, hWnd, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}



LRESULT CALLBACK MainWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hWorkspace = NULL;

    switch (message)
    {
    case WM_CREATE:
        {
            HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

            RECT rct;
            GetClientRect(hWnd, &rct);

            hWorkspace = CreateWindowEx(WS_EX_CLIENTEDGE,
                                        g_workspaceWinClassName,
                                        L"",
                                        WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
                                        120, 20,
                                        rct.right - rct.left - 140, rct.bottom - rct.top - 40,
                                        hWnd, NULL,
                                        hInstance, NULL);
        }
        break;

    case WM_SIZE:
        {
            int clientWidth = LOWORD(lParam);
            int clientHeight = HIWORD(lParam);

            SetWindowPos(hWorkspace, HWND_TOP, 0, 0, clientWidth - 140, clientHeight - 40, SWP_NOMOVE|SWP_NOREPOSITION);
        }
        break;


    case WM_MOUSEWHEEL:
        SendMessage(hWorkspace, message, wParam, lParam);
        break;


    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}



LRESULT CALLBACK WorkspaceProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HINSTANCE hInstance = NULL;
    static HDC hCanvasDC = NULL;
    static HBITMAP hCanvasBMP = NULL;
    static HBITMAP hOldBMP = NULL;

    static int canvasWidth = stoi(g_options["canvas_width"]);
    static int canvasHeight = stoi(g_options["canvas_height"]);

    static RECT clippingRect;
    static RECT targetRect;

    static POINT mouseDelta = {0, 0};
    static POINT mousePos = {0, 0};

    static float scaleFactor = 1;

    switch (message)
    {
    case WM_CREATE:
        {
            hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
            
            HDC hCurrentDC = GetDC(hWnd);

            hCanvasDC = CreateCompatibleDC(hCurrentDC);
            hCanvasBMP = CreateCompatibleBitmap(hCurrentDC, canvasWidth, canvasHeight);

            hOldBMP = (HBITMAP)SelectObject(hCanvasDC, hCanvasBMP);

            ReleaseDC(hWnd, hCurrentDC);

            clippingRect.left = 0;
            clippingRect.top = 0;
            clippingRect.right = canvasWidth;
            clippingRect.bottom = canvasHeight;

        }
        break;


    case WM_PAINT:
        {
            RECT client;
            GetClientRect(hWnd, &client);


            PAINTSTRUCT ps;

            HDC hPaintDC = BeginPaint(hWnd, &ps);


            HBITMAP circlesBMP = LoadBitmap(hInstance, MAKEINTRESOURCE(ID_CIRCLES_BMP));
            HBRUSH circlesBrush = CreatePatternBrush(circlesBMP);

            HBRUSH oldBrush = (HBRUSH)SelectObject(hCanvasDC, circlesBrush);
            Rectangle(hCanvasDC, 0, 0, canvasWidth, canvasHeight);

            SelectObject(hCanvasDC, oldBrush);
            DeleteObject(circlesBrush);
            DeleteObject(circlesBMP);


            SetStretchBltMode(hPaintDC, HALFTONE);
            StretchBlt(hPaintDC, 0, 0, 400, 400, hCanvasDC,
                        clippingRect.left,
                        clippingRect.top,
                        clippingRect.right - clippingRect.left,
                        clippingRect.bottom - clippingRect.top,
                        SRCCOPY);

            HBRUSH whiteBrush = CreateSolidBrush(RGB(0,0,0));
            oldBrush = (HBRUSH)SelectObject(hCanvasDC, whiteBrush);

            

            Rectangle(hCanvasDC, 0, 0, client.right - client.left, client.bottom - client.top);

            SelectObject(hCanvasDC, oldBrush);
            DeleteObject(whiteBrush);

            EndPaint(hWnd, &ps);
        }
        break;


    case WM_ERASEBKGND:
        return 1;
        break;


    case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            mouseDelta.x = x - mousePos.x;
            mouseDelta.y = y - mousePos.y;
            mousePos.x = x;
            mousePos.y = y;

            if (wParam == MK_LBUTTON)
            {
                cout << "LMB DOWN\n";
                clippingRect.left -= mouseDelta.x;
                clippingRect.top -= mouseDelta.y;
                clippingRect.right -= mouseDelta.x;
                clippingRect.bottom -= mouseDelta.y;
                InvalidateRect(hWnd, NULL, TRUE);
            }


        }
        break;


    case WM_MOUSEWHEEL:
        {
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (delta > 0)
            {
                scaleFactor += 0.05f;

                clippingRect.left = min(clippingRect.left + 20, clippingRect.right - 50);
                clippingRect.top = min(clippingRect.top + 20, clippingRect.bottom - 50);

                clippingRect.right = max(clippingRect.left + 50, clippingRect.right -20);
                clippingRect.bottom = max(clippingRect.top + 50, clippingRect.bottom - 20);
            }
            else
            {
                scaleFactor -= 0.05f;

                clippingRect.left = max(0, clippingRect.left - 20);
                clippingRect.top = max(0, clippingRect.top - 20);

                clippingRect.right = min(canvasWidth, clippingRect.right + 20);
                clippingRect.bottom = min(canvasWidth, clippingRect.bottom + 20);
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

/*
    case WM_VSCROLL:
        if (!lParam) // check if the message is from standard scrollbar
        {
            SCROLLINFO scr;
            scr.cbSize = sizeof(SCROLLINFO);
            scr.fMask = SIF_POS|SIF_RANGE|SIF_PAGE;

            GetScrollInfo(hWnd, SB_VERT, &scr);

            int currScrollY = scr.nPos;
            int maxLimit = scr.nMax - scr.nPage + 1;

            switch (LOWORD(wParam))
            {
            case SB_LEFT:
                currScrollY = 0;
                break;

            case SB_RIGHT:
                currScrollY = maxLimit;
                break;

            case SB_LINELEFT:                       
                currScrollY = max(0, scr.nPos-5);
                break;

            case SB_PAGELEFT:
                {
                    RECT rct;
                    GetClientRect(hWnd, &rct);
                    currScrollY = max(0, scr.nPos - (rct.bottom -rct.top));
                }               
                break;

            case SB_LINERIGHT:
                currScrollY = min(maxLimit, scr.nPos+5);
                break;                  

            case SB_PAGERIGHT:
                {
                    RECT rct;
                    GetClientRect(hWnd, &rct);
                    currScrollY = min(maxLimit, scr.nPos + (rct.bottom -rct.top));
                }               
                break;

            case SB_THUMBTRACK:
            case SB_THUMBPOSITION:
                {                   
                    currScrollY = HIWORD(wParam);
                }
                break;

            default:
                break;
            }

            ScrollWindowEx(hWnd, prevScrollY - currScrollY, 0, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
            prevScrollY = currScrollY;
            scr.nPos = prevScrollY;
            SetScrollInfo(hWnd, SB_VERT, &scr, TRUE);
        }
        break;
        */

    case WM_DESTROY:
        if (hOldBMP) { SelectObject(hCanvasDC, hOldBMP); }
        if (hCanvasBMP) { DeleteObject(hCanvasBMP); }
        if (hCanvasDC) { DeleteDC(hCanvasDC); }
        break;


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}



ATOM RegisterMainWindow(HINSTANCE hInstance, LPCWSTR className, WNDPROC proc)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.lpfnWndProc = proc;
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = className;
    wcex.lpszClassName  = className;
    wcex.hIconSm        = 0;

    return RegisterClassEx(&wcex);
}


void CreateDeveloperConsole()
{
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    int consoleHandleR, consoleHandleW ;
    long stdioHandle;
    FILE *fptr;

    AllocConsole();
    std::wstring strW = L"Dev Console";
    SetConsoleTitle( strW.c_str() );

    EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE , MF_GRAYED);
    DrawMenuBar(GetConsoleWindow());

    GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo );

    stdioHandle = (long)GetStdHandle( STD_INPUT_HANDLE );
    consoleHandleR = _open_osfhandle( stdioHandle, _O_TEXT );
    fptr = _fdopen( consoleHandleR, "r" );
    *stdin = *fptr;
    setvbuf( stdin, NULL, _IONBF, 0 );

    stdioHandle = (long) GetStdHandle( STD_OUTPUT_HANDLE );
    consoleHandleW = _open_osfhandle( stdioHandle, _O_TEXT );
    fptr = _fdopen( consoleHandleW, "w" );
    *stdout = *fptr;
    setvbuf( stdout, NULL, _IONBF, 0 );

    stdioHandle = (long)GetStdHandle( STD_ERROR_HANDLE );
    *stderr = *fptr;
    setvbuf( stderr, NULL, _IONBF, 0 );
}



bool LoadConfigFile(char const * filePath)
{
    ifstream configFile(filePath, ifstream::in);

    if (configFile.fail())
    {
        return false;
    }

    regex rgx("(\\w+)\\s*=\\s*(\\w+)");
    smatch results;

    while (configFile.good())
    {
        string str;
        getline(configFile, str);       

        if (regex_search(str, results, rgx))
        {
            // cout << results[1] << " <-> " << results[2] << endl;
            g_options[results.str(1)] = results.str(2);
        }
    }

    return true;
}

