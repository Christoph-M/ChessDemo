#ifndef SQUARE_H
#define SQUARE_H

#include <windows.h>

class square
{
    public:
        square();

        void drawBitmap(HDC&, HDC&, HBITMAP&) const;

        void setPosX(int i)     { _posX = i; };
        void setPosY(int i)     { _posY = i; };
        void setColor(bool i)   { _isWhite = i; };
        void setVisible(bool i) { _isVisible = i; };

        const int  getPosX()    const { return _posX; };
        const int  getPosY()    const { return _posY; };
        const bool getColor()   const { return _isWhite; };
        const bool getVisible() const { return _isVisible; };

        virtual ~square();
    protected:
        int  _posX, _posY;
        bool _isWhite, _isVisible;
    private:
};

#endif // SQUARE_H
