#pragma comment( lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")



#include <windows.h>
#include <commctrl.h>
#include <map>
#include <string>


using namespace std;


map<string, string> g_options;



LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegisterMainWindow(HINSTANCE, LPCWSTR);
bool LoadConfigFile(char const * filePath);

void CreateDeveloperConsole();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    LPCWSTR mainWinClassName = L"lab2";
    HWND hWnd;
    MSG msg;

    INITCOMMONCONTROLSEX ctls;
    ctls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    ctls.dwICC = ICC_BAR_CLASSES;

    InitCommonControlsEx(&ctls);

    // CreateDeveloperConsole();


    if (!LoadConfigFile("settings.cfg"))
    {
        MessageBox(NULL, L"Could not load config file", L"Error", MB_OK);
    }


    if (!RegisterMainWindow(hInstance, mainWinClassName))
    {
        MessageBox(NULL, L"Could not register window class!", L"Error!", MB_OK|MB_ICONERROR);
        return 0;
    }

    RECT rct;

    rct.left = 0;
    rct.top = 0;
    rct.right = stoi(g_options["window_width"]);
    rct.bottom = stoi(g_options["window_height"]);

    AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, TRUE, WS_EX_OVERLAPPEDWINDOW);

    hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                            mainWinClassName,
                            L"WP Lab 2",
                            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_HSCROLL | WS_VSCROLL, //WS_OVERLAPPEDWINDOW,
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

