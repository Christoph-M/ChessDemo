#include "square.h"

square::square()
{
    //ctor
}

//__________________Draw bitmap without transparency_____________________
void square::drawBitmap(HDC & memHdc, HDC & bmpHdc, HBITMAP & hBitmap) const{
    BITMAP bm;
    SelectObject(bmpHdc, hBitmap);
    GetObject(hBitmap, sizeof(bm), &bm);
    BitBlt(memHdc, _coordX, _coordY, bm.bmWidth, bm.bmHeight, bmpHdc, 0, 0, SRCCOPY);
}

//__________________Draw bitmap with transparency_____________________
void square::drawBitmap(HDC & memHdc, HDC & bmpHdc, HBITMAP & hBitmap, HBITMAP & hMask) const{
    BITMAP bm;
    SelectObject(bmpHdc, hMask);
    GetObject(hBitmap, sizeof(bm), &bm);
    BitBlt(memHdc, _coordX, _coordY, bm.bmWidth, bm.bmHeight, bmpHdc, 0, 0, SRCAND);
    SelectObject(bmpHdc, hBitmap);
    BitBlt(memHdc, _coordX, _coordY, bm.bmWidth, bm.bmHeight, bmpHdc, 0, 0, SRCPAINT);
}

//__________________Check if cursor is within chessboard_____________________
const bool square::cursorWithinBoard(POINT & p) const{
    if ((p.x >= BOARD_EDGE && p.x <= this->_calcPos(BOARD_WIDTH)) && (p.y >= BOARD_EDGE && p.y <= this->_calcPos(BOARD_HEIGHT))){
        return true;
    } else {
        return false;
    }
}

//__________________Check if cursor is within square_____________________
const bool square::cursorWithinSquare(POINT & p) const{
    if ((p.x > _coordX && p.x < (_coordX + 64)) && (p.y > _coordY && p.y < (_coordY + 64))){
        return true;
    } else {
        return false;
    }
}

//__________________Set position of square_____________________
void square::setPos(int x, int y){
    _posX = x;
    _coordX = this->_calcPos(x);

    _posY = y;
    _coordY = this->_calcPos(y);
}

//__________________Set contained ID of square_____________________
void square::setContID(int i){
    if (i){
        _contID = i;
        _hasID = true;
    } else {
        _hasID = false;
    }
}

square::~square()
{
    puts("Destructor call for square Object\n"); //dtor
}
