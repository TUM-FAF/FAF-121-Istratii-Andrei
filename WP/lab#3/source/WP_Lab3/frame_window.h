#pragma once


#include <windows.h>
#include "window.h"
#include "colorpicker.h"
#include "workspace_window.h"
#include "toolbar.h"




class FrameWindow : public Window
{
private:
    WorkspaceWindow workspace;
    Colorpicker fillColorPicker;
    Colorpicker strokeColorPicker;
    Toolbar toolbar;

    COLORREF customColors[16];

    virtual void OnCreate();
    virtual void OnSize(int width, int height, WPARAM wParam);
    virtual void OnCommand(WPARAM wParam, LPARAM lParam);

    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void FillWndClassEx(WNDCLASSEX & wcex);
};