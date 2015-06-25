#include "wApp.h"

wApp::wApp(HINSTANCE hThisInstance, char * szClassName, WNDPROC WindowProcedure, LPCSTR menuName)
{
    /* The Window structure */
    _wndClsEx.hInstance     = hThisInstance;
    _wndClsEx.lpszClassName = szClassName;
    _wndClsEx.lpfnWndProc   = WindowProcedure;        /* This function is called by windows */
    _wndClsEx.style         = CS_DBLCLKS;             /* Catch double-clicks */
    _wndClsEx.cbSize        = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    _wndClsEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    _wndClsEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    _wndClsEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
    _wndClsEx.lpszMenuName  = menuName;               /* No menu */
    _wndClsEx.cbClsExtra    = 0;                      /* No extra bytes after the window class */
    _wndClsEx.cbWndExtra    = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    _wndClsEx.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
}

bool wApp::regWnd() const{
    return RegisterClassEx(&_wndClsEx);
}

wApp::~wApp()
{
    printf("Destructor call for wApp Object\n");
}
