
#pragma once

#include <windows.h>

class Toolbar
{
public:
    Toolbar();
    ~Toolbar();
    void Create(DWORD style, HWND hParent, HINSTANCE hInstance);

private:
    HWND hWnd;

};