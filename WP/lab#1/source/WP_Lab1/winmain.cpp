
#pragma comment( lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")



#include <windows.h>
#include <commctrl.h>
#include <io.h>
#include <cstdio>
#include <fcntl.h>
#include <iostream>

#include "resources.h"


using namespace std;




LRESULT CALLBACK GlobalWndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegisterMainWindow(HINSTANCE, LPCWSTR);
void CreateDeveloperConsole();





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    LPCWSTR mainWinClassName = L"lab1";
    HWND hWnd;
    MSG msg;

    // CreateDeveloperConsole();

    // cout << "DevConsole initialized" << endl;


    if (!RegisterMainWindow(hInstance, mainWinClassName))
    {
        MessageBox(NULL, L"Could not register window class!", L"Error!", MB_OK|MB_ICONERROR);
        return 0;
    }

    RECT rct;

    rct.left = 0;
    rct.top = 0;
    rct.right = 400;
    rct.bottom = 200;

    AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);

    hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                            mainWinClassName,
                            L"WP Lab 1",
                            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX, //WS_OVERLAPPEDWINDOW,
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
    static HFONT hFont;
    static HFONT hFontForDesktop;
    static int bgColor = 0;


    switch (message)
    {
    case WM_CREATE:
        {
            RECT rct;
            GetClientRect(hWnd, &rct);

            HINSTANCE hInctance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);


            CreateWindow(L"STATIC", L"Default Stuff:",
                            WS_CHILD|WS_VISIBLE,
                            20, 20, 150, 23,
                            hWnd, (HMENU)DEFAULT_STATIC, hInctance, NULL);


            CreateWindow(L"BUTTON", L"Big Text",
                            WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                            20, 70, 150, 30,
                            hWnd, (HMENU)DEFAULT_BUTTON, hInctance, NULL);


            CreateWindowEx(WS_EX_CLIENTEDGE,
                            L"EDIT", L"",
                            WS_CHILD|WS_VISIBLE,
                            20, 130, 150, 23,
                            hWnd, (HMENU)DEFAULT_EDIT, hInctance, NULL);


            CreateWindowEx(WS_EX_STATICEDGE,
                            L"STATIC", L"Enhanced Stuff:",
                            WS_CHILD|WS_VISIBLE|SS_CENTER,
                            220, 20, 150, 23,
                            hWnd, (HMENU)ENHANCED_STATIC, hInctance, NULL);


            CreateWindowEx(WS_EX_TRANSPARENT,
                            L"BUTTON", L"Change BG Color",
                            WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                            220, 70, 150, 30,
                            hWnd, (HMENU)ENHANCED_BUTTON, hInctance, NULL);


            CreateWindowEx(WS_EX_WINDOWEDGE,
                            L"EDIT", L"",
                            WS_CHILD|WS_VISIBLE|WS_BORDER,
                            220, 130, 150, 23,
                            hWnd, (HMENU)ENHANCED_EDIT, hInctance, NULL);


            hFont = CreateFont(20, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Calibri");

            SendMessage(GetDlgItem(hWnd, ENHANCED_STATIC), WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(GetDlgItem(hWnd, ENHANCED_BUTTON), WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(GetDlgItem(hWnd, ENHANCED_EDIT), WM_SETFONT, (WPARAM)hFont, TRUE);

            hFontForDesktop = CreateFont(300, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Arial");
            
        }
        break;
    // END WM_CREATE


    case WM_PAINT:
        {
            PAINTSTRUCT ps;         

            HDC hDC = BeginPaint(hWnd, &ps);

                HBITMAP hBackgroundImg;
                if (bgColor == 0)
                {
                    hBackgroundImg = (HBITMAP)LoadImage(NULL, L"circles_small.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                }
                else
                {
                    hBackgroundImg = (HBITMAP)LoadImage(NULL, L"circles_small_blue.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                }

                if (!hBackgroundImg)
                {
                    MessageBox(hWnd,L"Err",L"Err",MB_OK);
                }
                HBITMAP hOldBm;

                HDC hMemDC = CreateCompatibleDC(hDC);

                hOldBm = (HBITMAP)SelectObject(hMemDC, hBackgroundImg);

                RECT rct;

                GetClientRect(hWnd, &rct);
                

                LONG width = rct.right - rct.left;
                LONG height = rct.bottom - rct.top;

                LONG x = width/2;
                while (x < width)
                {
                    LONG y = 0;
                    while (y < height)
                    {
                        BitBlt(hDC, x, y, 100,100, hMemDC, 0, 0, SRCCOPY);
                        y += 100;
                    }
                    x += 100;
                }

                SelectObject(hMemDC, hOldBm);
                DeleteObject(hBackgroundImg);

                DeleteDC(hMemDC);

            EndPaint(hWnd, &ps);
        }
        break;



    case WM_COMMAND:
        switch (HIWORD(wParam))
        {
        case BN_CLICKED:
            switch (LOWORD(wParam))
            {
            case DEFAULT_BUTTON:
                {
                    wchar_t text[128];
                    GetWindowText(GetDlgItem(hWnd, DEFAULT_EDIT), text, 128);
                    HDC hDesktopDC = GetDC(NULL);
                    if (hDesktopDC)
                    {
                        HFONT hOldFont = (HFONT)SelectObject(hDesktopDC, hFontForDesktop);
                        TextOut(hDesktopDC, 200, 200, text, lstrlen(text));
                        SelectObject(hDesktopDC, hOldFont);
                        ReleaseDC(NULL, hDesktopDC);
                    }
                    SetTimer(hWnd, CLEARSCREEN_TIMER, 1000, NULL);
                }
                break;

            case ENHANCED_BUTTON:
                if (bgColor == 0)
                {
                    bgColor = 1;
                }
                else
                {
                    bgColor = 0;
                }
                InvalidateRect(hWnd, NULL, TRUE);
                break;

            default:
                break;
            }
            break;
        // END BN_CLICKED


        case EN_CHANGE:
            switch (LOWORD(wParam))
            {
            case DEFAULT_EDIT:
                wchar_t text[128];
                GetWindowText((HWND)lParam, text, 128);
                SetWindowText(GetDlgItem(hWnd, DEFAULT_STATIC), text);
                break;

            case ENHANCED_EDIT:
                {
                    wchar_t text[128];
                    wchar_t exitText[] = L"exit";
                    GetWindowText((HWND)lParam, text, 128);
                    if (lstrcmp(text, exitText) == 0)
                    {
                        DestroyWindow(hWnd);
                    }
                }
                break;

            default:
                break;
            }
            break;

        case EN_KILLFOCUS:
            if (LOWORD(wParam) == DEFAULT_EDIT)
            {
                SetWindowText(GetDlgItem(hWnd, DEFAULT_STATIC), L"Default Stuff:");
            }           
            break;

        default:
            break;
        }
        break;
    // END WM_COMMAND

    case WM_CTLCOLOREDIT:
        {
            HWND hEdit = (HWND)lParam;
            HDC hEditDC = (HDC)wParam;
            if (GetDlgCtrlID(hEdit) == ENHANCED_EDIT)
            {
                COLORREF color = bgColor == 0 ? RGB(255, 155, 125) : RGB(125, 180, 255);
                SetBkColor(hEditDC, color);
                return (LRESULT)GetStockObject(DC_BRUSH);
            }
            else
            {
                return DefWindowProc(hWnd, message, wParam, lParam);
            }

        }
        break;


    case WM_TIMER:
        InvalidateRect(NULL, NULL, NULL);
        KillTimer(hWnd, (UINT_PTR)wParam);
        break;


    case WM_SIZE:
        {
            int x = LOWORD(lParam)/2 + 20;
            SetWindowPos(GetDlgItem(hWnd, ENHANCED_STATIC), HWND_TOP, x, 20, 0, 0, SWP_NOSIZE);
            SetWindowPos(GetDlgItem(hWnd, ENHANCED_BUTTON), HWND_TOP, x, 70, 0, 0, SWP_NOSIZE);
            SetWindowPos(GetDlgItem(hWnd, ENHANCED_EDIT), HWND_TOP, x, 130, 0, 0, SWP_NOSIZE);
        }
        break;


    case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO pMMInfo;
            pMMInfo = (LPMINMAXINFO)lParam;

            RECT rct;

            // min size
            rct.left = 0;
            rct.top = 0;
            rct.right = 400;
            rct.bottom = 200;

            AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);

            POINT minSize;
            minSize.x = rct.right - rct.left;
            minSize.y = rct.bottom - rct.top;

            pMMInfo->ptMinTrackSize = minSize;

            // max size
            rct.left = 0;
            rct.top = 0;
            rct.right = 800;
            rct.bottom = 400;

            AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);

            POINT maxSize;
            maxSize.x = rct.right - rct.left;
            maxSize.y = rct.bottom - rct.top;

            pMMInfo->ptMaxTrackSize = maxSize;
        }
        break;


    case WM_CLOSE:
        MessageBox(hWnd, L"In order to quit, type 'exit' in the form on the right.", L"Tip", MB_OK|MB_ICONINFORMATION);
        break;

    case WM_DESTROY:
        DeleteObject(hFontForDesktop);
        DeleteObject(hFont);
        PostQuitMessage(0);     
        break;


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



ATOM RegisterMainWindow(HINSTANCE hInstance, LPCWSTR className)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.lpfnWndProc = MainWinProc;
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = 0;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = 0;
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


