#include "highlight.h"

highlight::highlight()
{
    _posX = 0; //ctor
    _posY = 0;
}

void highlight::drawBitmap(HDC & memHdc, HDC & bmpHdc, HBITMAP & hBitmap, HBITMAP & hMask) const{
    BITMAP bm;
    SelectObject(bmpHdc, hMask);
    GetObject(hBitmap, sizeof(bm), &bm);
    BitBlt(memHdc, _posX, _posY, bm.bmWidth, bm.bmHeight, bmpHdc, 0, 0, SRCAND);
    SelectObject(bmpHdc, hBitmap);
    BitBlt(memHdc, _posX, _posY, bm.bmWidth, bm.bmHeight, bmpHdc, 0, 0, SRCPAINT);
}

highlight::~highlight()
{
    //dtor
}
