#ifndef WAPP_H
#define WAPP_H

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
