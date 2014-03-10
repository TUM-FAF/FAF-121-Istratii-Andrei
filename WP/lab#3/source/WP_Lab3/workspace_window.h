#pragma once


#include <windows.h>
#include "window.h"




class WorkspaceWindow : public Window
{
private:
    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void FillWndClassEx(WNDCLASSEX & wcex);
};