#pragma once


#include <windows.h>
#include "window.h"
#include "workspace_window.h"
#include "toolbar.h"




class FrameWindow : public Window
{
private:
    WorkspaceWindow workspace;
    Toolbar toolbar;

    virtual void OnCreate();

    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void FillWndClassEx(WNDCLASSEX & wcex);
};