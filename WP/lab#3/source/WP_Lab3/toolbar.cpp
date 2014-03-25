
#include "toolbar.h"
#include "shared_headers.h"


Toolbar::Toolbar()
{
    hWnd = NULL;
}

Toolbar::~Toolbar()
{

}


void Toolbar::Create(DWORD style, HWND hParent, HINSTANCE hInstance)
{
    TBBUTTON tbbuttons[3] = {
        {I_IMAGENONE, IDM_TBITEM1, TBSTATE_ENABLED|TBSTATE_WRAP, BTNS_BUTTON, {0}, 0L, 0},
        {I_IMAGENONE, IDM_TBITEM2, TBSTATE_ENABLED|TBSTATE_WRAP, BTNS_BUTTON, {0}, 0L, 0},
        {I_IMAGENONE, IDM_TBITEM3, TBSTATE_ENABLED|TBSTATE_WRAP, BTNS_BUTTON, {0}, 0L, 0}
    };


    hWnd = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
                    style, 0, 0, 0, 0,
                    hParent, NULL, hInstance, NULL);

    SendMessage(hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWnd, TB_ADDBUTTONS, 3, (LPARAM) &tbbuttons);
}
