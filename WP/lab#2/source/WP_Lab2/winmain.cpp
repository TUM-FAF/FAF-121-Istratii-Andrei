
#pragma comment( lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")



#include <windows.h>
#include <commctrl.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <regex>

#include <iostream>
#include <io.h>
#include <cstdio>
#include <fcntl.h>


#include "resource.h"


using namespace std;



struct WindowState 
{
	int width;
	int height;
	int bgColorScrollPos;
};


map<string, string> g_options;



LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SubClassListBoxProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
BOOL CALLBACK AboutProc (HWND, UINT, WPARAM, LPARAM);
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



LRESULT CALLBACK MainWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInctance;
	static HFONT hFont;
	static COLORREF bgColor;

	static vector<WindowState> windowStates;

	static int prevScrollX = 0;
	static int prevScrollY = 0;


	switch (message)
	{
	case WM_CREATE:
		{
			RegisterHotKey(hWnd, HK_SAVE_STATE, MOD_CONTROL, 0x53); // CTRL+S


			hInctance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

			hFont = CreateFont(20, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Calibri");

			HWND hListBox = CreateWindowEx(WS_EX_CLIENTEDGE,
											L"LISTBOX", NULL,
											WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_NOTIFY,
											10, 10, 150, 150,
											hWnd, (HMENU)ID_LISTBOX,
											hInctance, NULL);

			SetWindowSubclass(hListBox, SubClassListBoxProc, ID_LISTBOX, reinterpret_cast<DWORD_PTR>(&windowStates));

			SendMessage(hListBox, WM_SETFONT, (WPARAM)hFont, TRUE);


			// Create static text fields

			HWND hStatic = CreateWindow(L"STATIC", L" Client Width:",
										WS_CHILD|WS_VISIBLE|SS_LEFT,
										170, 10, 200, 25,
										hWnd, NULL,
										hInctance, NULL);

			SendMessage(hStatic, WM_SETFONT, (WPARAM)hFont, TRUE);

			hStatic = CreateWindow(L"STATIC", L" Client Height:",
									WS_CHILD|WS_VISIBLE|SS_LEFT,
									170, 60, 200, 25,
									hWnd, NULL,
									hInctance, NULL);

			SendMessage(hStatic, WM_SETFONT, (WPARAM)hFont, TRUE);

			hStatic = CreateWindow(L"STATIC", L" Background Light:",
									WS_CHILD|WS_VISIBLE|SS_LEFT,
									170, 110, 200, 25,
									hWnd, NULL,
									hInctance, NULL);
			
			SendMessage(hStatic, WM_SETFONT, (WPARAM)hFont, TRUE);


			// Setup Trackbars

			HWND hTrackbarWidth = CreateWindow(TRACKBAR_CLASS, NULL,
												WS_CHILD|WS_VISIBLE|TBS_HORZ,
												170, 35, 200, 20,
												hWnd, (HMENU)ID_TRACKBAR_WIDTH,
												hInctance, NULL);

			int min_width = stoi(g_options["min_window_width"]);
			int max_width = stoi(g_options["max_window_width"]);

			SendMessage(hTrackbarWidth, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(min_width, max_width));
			SendMessage(hTrackbarWidth, TBM_SETPAGESIZE, 0, (LPARAM) 40);
			SendMessage(hTrackbarWidth, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) stoi(g_options["window_width"]));
			
			
			HWND hTrackbarHeight = CreateWindow(TRACKBAR_CLASS, NULL,
												WS_CHILD|WS_VISIBLE|TBS_HORZ,
												170, 85, 200, 20,
												hWnd, (HMENU)ID_TRACKBAR_HEIGHT,
												hInctance, NULL);


			int min_height = stoi(g_options["min_window_height"]);
			int max_height = stoi(g_options["max_window_height"]);

			SendMessage(hTrackbarHeight, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(min_height, max_height));
			SendMessage(hTrackbarHeight, TBM_SETPAGESIZE, 0, (LPARAM) 40);
			SendMessage(hTrackbarHeight, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) stoi(g_options["window_height"]));


			// Setup custom scrollbar

			HWND hScroll = CreateWindow(L"SCROLLBAR", NULL,
										WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_TOPALIGN,
										170, 135, 200, 20,
										hWnd, (HMENU)ID_SCROLLBAR,
										hInctance, NULL);

			SCROLLINFO scr;
			scr.cbSize = sizeof(SCROLLINFO);
			scr.fMask = SIF_RANGE|SIF_POS;
			scr.nMin = 0;
			scr.nMax = 255;
			scr.nPos = 240;

			SetScrollInfo(hScroll, SB_CTL, &scr, TRUE);

			
			// Setup standard scrollbars

			scr.fMask = SIF_RANGE|SIF_DISABLENOSCROLL;
			scr.nMin = 0;
			scr.nMax = stoi(g_options["window_width"]) - GetSystemMetrics(SM_CXVSCROLL) -1;

			SetScrollInfo(hWnd, SB_HORZ, &scr, FALSE);

			scr.fMask = SIF_RANGE|SIF_DISABLENOSCROLL;
			scr.nMin = 0;
			scr.nMax = stoi(g_options["window_height"]) - GetSystemMetrics(SM_CYHSCROLL) -1;

			SetScrollInfo(hWnd, SB_VERT, &scr, FALSE);


			// Set initial Background color

			bgColor = GetSysColor(COLOR_WINDOW-1);
		}
		break;
	// END WM_CREATE


	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(LoadCursor(hInctance, MAKEINTRESOURCE(IDC_CURSOR1)));
			return TRUE;
		}
		break;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_APP_SAVE_STATE:
			{
				WindowState s;

				RECT rct;
				GetClientRect(hWnd, &rct);

				s.width = rct.right - rct.left;
				s.height = rct.bottom - rct.top;

				SCROLLINFO scr;

				scr.cbSize = sizeof(SCROLLINFO);
				scr.fMask = SIF_POS;

				GetScrollInfo(GetDlgItem(hWnd, ID_SCROLLBAR), SB_CTL, &scr);

				s.bgColorScrollPos = scr.nPos;

				wstring text;

				text += L"W:" + to_wstring(s.width);
				text += L" H:" + to_wstring(s.height);
				text += L" C:" + to_wstring(s.bgColorScrollPos);

				int res = SendMessage(GetDlgItem(hWnd, ID_LISTBOX), LB_ADDSTRING, NULL, (LPARAM)text.c_str());

				if (res == LB_ERR || res == LB_ERRSPACE)
				{
					MessageBox(hWnd, L"Could not save state in listbox.", L"Error!", MB_OK|MB_ICONERROR);
					break;
				}

				windowStates.push_back(s);
			}
			break;


		case IDM_APP_REMOVE_STATE:
			{
				int sel = SendMessage(GetDlgItem(hWnd, ID_LISTBOX), LB_GETCURSEL, 0, 0);
				if (sel != LB_ERR)
				{
					if (SendMessage(GetDlgItem(hWnd, ID_LISTBOX), LB_DELETESTRING, (WPARAM)sel, 0) != LB_ERR)
					{
						windowStates.erase(windowStates.begin() + sel);
					}
				}
			}			
			break;


		case IDM_APP_EXIT:
			SendMessage(hWnd, WM_CLOSE, NULL, NULL);
			break;


		case IDM_ABOUT:
			DialogBox(hInctance, L"AboutBox", hWnd, AboutProc);
			break;

		default:
			break;
		}

		switch (HIWORD(wParam))
		{
		case LBN_DBLCLK:
			{
				int sel = SendMessage((HWND)lParam, LB_GETCURSEL, 0, 0);
				if (sel != LB_ERR)
				{
					WindowState s = windowStates[sel];

					RECT rct;
					rct.left = 0;
					rct.top = 0;
					rct.right = s.width + GetSystemMetrics(SM_CXVSCROLL);
					rct.bottom = s.height + GetSystemMetrics(SM_CYHSCROLL);

					SendMessage(GetDlgItem(hWnd, ID_TRACKBAR_WIDTH), TBM_SETPOS, TRUE, rct.right);
					SendMessage(GetDlgItem(hWnd, ID_TRACKBAR_HEIGHT), TBM_SETPOS, TRUE, rct.bottom);

					AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW, TRUE, WS_EX_OVERLAPPEDWINDOW);
					SetWindowPos(hWnd, HWND_TOP, 0, 0, rct.right - rct.left, rct.bottom - rct.top, SWP_NOMOVE|SWP_NOREPOSITION);
					

					bgColor = RGB(s.bgColorScrollPos, s.bgColorScrollPos, s.bgColorScrollPos);
					SetScrollPos(GetDlgItem(hWnd, ID_SCROLLBAR), SB_CTL, s.bgColorScrollPos, TRUE);

					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			break;

		default:
			break;
		}
		break;


	case WM_HOTKEY:
		switch (wParam)
		{
		case HK_SAVE_STATE: // CTRL+S
			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDM_APP_SAVE_STATE, 0), 0);
			break;

		default:
			break;
		}
		break;


	case WM_SIZE:
		{
			int clientWidth = LOWORD(lParam);
			int clientHeight = HIWORD(lParam);

			SCROLLINFO scr;
			scr.cbSize = sizeof(SCROLLINFO);
			scr.fMask = SIF_PAGE | SIF_DISABLENOSCROLL;

			scr.nPage = clientWidth;
			SetScrollInfo(hWnd, SB_HORZ, &scr, TRUE);

			scr.nPage = clientHeight;
			SetScrollInfo(hWnd, SB_VERT, &scr, TRUE);


			scr.fMask = SIF_POS;

			GetScrollInfo(hWnd, SB_HORZ, &scr);
			int currScrollX = scr.nPos;

			GetScrollInfo(hWnd, SB_VERT, &scr);
			int currScrollY = scr.nPos;

			ScrollWindowEx(hWnd, prevScrollX - currScrollX, prevScrollY - currScrollY,
							NULL, NULL, NULL, NULL,
							SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);

			prevScrollX = currScrollX;
			prevScrollY = currScrollY;
		}
		break;


	case WM_HSCROLL:
		if (lParam) // check if a control-type scrollbar sent the message
		{
			int scrollerID = GetDlgCtrlID((HWND)lParam);

			switch (scrollerID)
			{
			case ID_SCROLLBAR:
				{
					SCROLLINFO scr;
					scr.cbSize = sizeof(SCROLLINFO);
					scr.fMask = SIF_POS|SIF_RANGE;

					GetScrollInfo((HWND)lParam, SB_CTL, &scr);

					switch (LOWORD(wParam))
					{
					case SB_LEFT:
						scr.nPos = 0;
						break;

					case SB_RIGHT:
						scr.nPos = 255;
						break;

					case SB_LINELEFT:						
						scr.nPos = max(scr.nMin, scr.nPos-5);
						break;

					case SB_PAGELEFT:
						scr.nPos = max(scr.nMin, scr.nPos-50);
						break;

					case SB_LINERIGHT:
						scr.nPos = min(scr.nMax, scr.nPos+5);
						break;					

					case SB_PAGERIGHT:
						scr.nPos = min(scr.nMax, scr.nPos+50);
						break;

					case SB_THUMBTRACK:		// fall through
					case SB_THUMBPOSITION:
						scr.nPos = (int)HIWORD(wParam);
						break;

					default:
						break;
					}

					bgColor = RGB(scr.nPos, scr.nPos, scr.nPos);
					InvalidateRect(hWnd, NULL, TRUE);

					scr.fMask = SIF_POS;
					SetScrollInfo((HWND)lParam, SB_CTL, &scr, TRUE);
				}				
				break;

			case ID_TRACKBAR_HEIGHT:
			case ID_TRACKBAR_WIDTH:
				{
					RECT rct;
					rct.left = 0;
					rct.top = 0;
					rct.right = SendMessage(GetDlgItem(hWnd, ID_TRACKBAR_WIDTH), TBM_GETPOS, 0, 0);
					rct.bottom = SendMessage(GetDlgItem(hWnd, ID_TRACKBAR_HEIGHT), TBM_GETPOS, 0, 0);

					AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW, TRUE, WS_EX_OVERLAPPEDWINDOW);

					SetWindowPos(hWnd, HWND_TOP, 0, 0, rct.right - rct.left, rct.bottom - rct.top, SWP_NOMOVE|SWP_NOREPOSITION);
				}		
				break;

			default:
				break;
			}			
		}
		else
		{
			SCROLLINFO scr;
			scr.cbSize = sizeof(SCROLLINFO);
			scr.fMask = SIF_POS|SIF_RANGE|SIF_PAGE;

			GetScrollInfo(hWnd, SB_HORZ, &scr);

			int currScrollX = scr.nPos;
			int maxLimit = scr.nMax - scr.nPage + 1;

			switch (LOWORD(wParam))
			{
			case SB_LEFT:
				currScrollX = 0;
				break;

			case SB_RIGHT:
				currScrollX = maxLimit;
				break;

			case SB_LINELEFT:						
				currScrollX = max(0, scr.nPos-5);
				break;

			case SB_PAGELEFT:
				{
					RECT rct;
					GetClientRect(hWnd, &rct);
					currScrollX = max(0, scr.nPos - (rct.right -rct.left));
				}				
				break;

			case SB_LINERIGHT:
				currScrollX = min(maxLimit, scr.nPos+5);
				break;					

			case SB_PAGERIGHT:
				{
					RECT rct;
					GetClientRect(hWnd, &rct);
					currScrollX = min(maxLimit, scr.nPos + (rct.right -rct.left));
				}				
				break;

			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:
				{					
					currScrollX = HIWORD(wParam);
				}
				break;

			default:
				break;
			}

			ScrollWindowEx(hWnd, prevScrollX - currScrollX, 0, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
			prevScrollX = currScrollX;
			scr.nPos = prevScrollX;
			SetScrollInfo(hWnd, SB_HORZ, &scr, TRUE);
		}		
		break;


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


	case WM_ERASEBKGND:
		{
			HPEN hPen = CreatePen(PS_SOLID, 1, bgColor);
			HBRUSH hBrush = CreateSolidBrush(bgColor);

			HDC hDC = (HDC)wParam;

			HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			RECT rct;

			GetClientRect(hWnd, &rct);

			Rectangle(hDC, rct.left, rct.top, rct.right, rct.bottom);

			SelectObject(hDC, hOldPen);
			SelectObject(hDC, hOldBrush);

			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
		break;


	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;


	case WM_DESTROY:
		DeleteObject(hFont);
		UnregisterHotKey(hWnd, HK_SAVE_STATE);
		PostQuitMessage(0);		
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



LRESULT CALLBACK SubClassListBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam,
									UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DELETE:
			{
				SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(IDM_APP_REMOVE_STATE, 0), 0);
			}
			break;

		default:
			break;
		}
		break;

	default:
		return DefSubclassProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



BOOL CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}



ATOM RegisterMainWindow(HINSTANCE hInstance, LPCWSTR className)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.lpfnWndProc = MainWinProc;
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= className;
	wcex.lpszClassName	= className;
	wcex.hIconSm		= 0;

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



