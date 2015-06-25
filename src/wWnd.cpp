#include "wWnd.h"

wWnd::wWnd()
{
    _hwnd = NULL;
}

HWND wWnd::create(HINSTANCE hThisInstance,
                  LPCSTR clsName,
                  LPCSTR wndName,
                  int boardWidth,
                  int boardHeight,
                  DWORD dxStyle,
                  DWORD dStyle,
                  int x, int y,
                  HWND parent,
                  HMENU hMenu,
                  LPVOID lpParam){

    _hwnd = CreateWindowEx(dxStyle, clsName, wndName, dStyle,
                           x, y, boardWidth, boardHeight, parent, hMenu, hThisInstance, lpParam);

    if (_hwnd == NULL) return NULL;

    return _hwnd;
}

bool wWnd::show(int nCmdShow) const{
    return ShowWindow(_hwnd, nCmdShow);
}

wWnd::operator HWND() const{
    return _hwnd;
}

wWnd::~wWnd()
{
    printf("Destructor call for wWnd Object\n");
}
