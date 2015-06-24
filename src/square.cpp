#include "square.h"

square::square():
    _posX(0),
    _posY(0)
{
    //ctor
}

void square::drawBitmap(HDC memHdc, HDC bmpHdc, HBITMAP hBitmap) const{
    BITMAP bm;
    SelectObject(bmpHdc, hBitmap);
    GetObject(hBitmap, sizeof(bm), &bm);
    BitBlt(memHdc, _posX, _posY, bm.bmWidth, bm.bmHeight, bmpHdc, 0, 0, SRCCOPY);
}

square::~square()
{
    //dtor
}
