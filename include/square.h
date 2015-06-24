#ifndef SQUARE_H
#define SQUARE_H

#include <windows.h>

class square
{
    public:
        square();

        void drawBitmap(HDC, HDC, HBITMAP) const;

        void setPosX(int i) { _posX = i; };
        void setPosY(int i) { _posY = i; };

        const int getPosX() const { return _posX; };
        const int getPosY() const { return _posY; };

        virtual ~square();
    protected:
        int _posX, _posY;
    private:
};

#endif // SQUARE_H
