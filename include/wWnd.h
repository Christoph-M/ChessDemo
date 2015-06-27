#ifndef WWND_H
#define WWND_H

/// Diese Klasse wurde zum größten Teil aus diesem Tutorial übernommen: http://www.functionx.com/win32/Lesson06.htm

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

        bool show(int nCmdShow = SW_SHOWNORMAL) const;

        operator HWND() const;

        virtual ~wWnd();
    protected:
        HWND _hwnd;
    private:
};

#endif // WWND_H
