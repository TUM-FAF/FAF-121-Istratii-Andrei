#pragma comment( lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")



#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <map>
#include <string>
#include <iostream>



#include "window.h"
#include "frame_window.h"
#include "workspace_window.h"
#include "tools.h"

#include "resource.h"

using namespace std;


map<string, string> g_options;
LPCWSTR g_mainWinClassName = L"lab3";
LPCWSTR g_workspaceWinClassName = L"workspace";




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX ctls;
    ctls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    ctls.dwICC = ICC_BAR_CLASSES;

    InitCommonControlsEx(&ctls);

    CreateDeveloperConsole();


    if (!LoadConfigFile("settings.cfg", g_options))
    {
        MessageBox(NULL, L"Could not load config file", L"Error", MB_OK);
        return 0;
    }

    RECT rct;

    rct.left = 0;
    rct.top = 0;
    rct.right = stoi(g_options["window_width"]);
    rct.bottom = stoi(g_options["window_height"]);

    AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, TRUE, WS_EX_OVERLAPPEDWINDOW);

    FrameWindow frameWindow;

    bool res = frameWindow.CreateEx(WS_EX_OVERLAPPEDWINDOW,
                                    g_mainWinClassName,
                                    L"WP Lab 3",
                                    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                                    CW_USEDEFAULT, 0,
                                    rct.right - rct.left, rct.bottom - rct.top,
                                    NULL, NULL,
                                    hInstance);
    

    if (!res)
    {
        MessageBox(NULL, L"Could not create the window!", L"Error!", MB_OK|MB_ICONERROR);
        return 0;
    }

    frameWindow.Show(nCmdShow);
    frameWindow.Update();

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
