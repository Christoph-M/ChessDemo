#ifndef SQUARE_H
#define SQUARE_H

#include <windows.h>

#include "header.h"

class square
{
    public:
        square();

        void       drawBitmap(HDC&, HDC&, HBITMAP&)           const;
        void       drawBitmap(HDC&, HDC&, HBITMAP&, HBITMAP&) const;
        const bool cursorWithinBoard(POINT&)                  const;
        const bool cursorWithinSquare(POINT&)                 const;

        void setPosX(int i)     { _posX = i; _coordX = this->_calcPos(i); };
        void setPosY(int i)     { _posY = i; _coordY = this->_calcPos(i); };
        void setColor(bool i)   { _isWhite = i; };
        void setVisible(bool i) { _isVisible = i; };

        const int  getPosX()    const { return _posX; };
        const int  getPosY()    const { return _posY; };
        const int  getCoordX()  const { return _coordX; };
        const int  getCoordY()  const { return _coordY; };
        const bool getColor()   const { return _isWhite; };
        const bool getVisible() const { return _isVisible; };

        virtual ~square();
    protected:
        int  _posX, _posY, _coordX, _coordY;
        bool _isWhite, _isVisible;

        const int _calcPos(int i) const { return (i * 64) + BOARD_EDGE; };  // Calcuation of position based on the grid position
    private:
};

#endif // SQUARE_H
