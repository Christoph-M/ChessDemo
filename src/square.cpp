#include "square.h"

square::square()
{
    //ctor
}

void square::drawBitmap(HDC & memHdc, HDC & bmpHdc, HBITMAP & hBitmap) const{
    BITMAP bm;
    SelectObject(bmpHdc, hBitmap);
    GetObject(hBitmap, sizeof(bm), &bm);
    BitBlt(memHdc, _posX, _posY, bm.bmWidth, bm.bmHeight, bmpHdc, 0, 0, SRCCOPY);
}

square::~square()
{
    //dtor
}
