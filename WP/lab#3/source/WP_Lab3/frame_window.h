#pragma once


#include <windows.h>
#include "window.h"
#include "workspace_window.h"




class FrameWindow : public Window
{
private:
    WorkspaceWindow workspace;

    virtual void OnCreate();

    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void FillWndClassEx(WNDCLASSEX & wcex);
};