#ifndef WAPP_H
#define WAPP_H

/// Diese Klasse wurde zum größten Teil aus diesem Tutorial übernommen: http://www.functionx.com/win32/Lesson06.htm

#include <windows.h>
#include <cstdio>

class wApp
{
    public:
        wApp(HINSTANCE, char *, WNDPROC, LPCSTR menuName = NULL);

        bool regWnd() const;

        virtual ~wApp();
    protected:
        WNDCLASSEX _wndClsEx;
    private:
        wApp();
};

#endif // WAPP_H
