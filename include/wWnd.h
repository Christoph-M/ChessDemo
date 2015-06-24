#ifndef WWND_H
#define WWND_H

#include <windows.h>
#include <cstdio>
#include <typeinfo>

class wWnd
{
    public:
        wWnd();

        HWND create(HINSTANCE,
                    LPCSTR,
                    LPCSTR,
                    int boardWidth  = 8,
                    int boardHeight = 8,
                    DWORD dxStyle   = 0,
                    DWORD dStyle    = WS_SYSMENU | WS_MINIMIZEBOX,
                    int x           = CW_USEDEFAULT,
                    int y           = CW_USEDEFAULT,
                    HWND parent     = HWND_DESKTOP,
                    HMENU hMenu     = NULL,
                    LPVOID lpParam  = NULL);

        bool show(int nCmdShow = SW_SHOWNORMAL);

        operator HWND();

        virtual ~wWnd();
    protected:
        HWND _hwnd;
    private:
};

#endif // WWND_H
