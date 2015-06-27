#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include <square.h>


class highlight : public square
{
    public:
        highlight();

        void setPos(int x, int y) { _posX = x; _coordX = this->_calcPos(x) - 4;
                                    _posY = y; _coordY = this->_calcPos(y) - 4; };
        void setPosX(int i)  { _posX = i; _coordX = this->_calcPos(i) - 4; };
        void setPosY(int i)  { _posY = i; _coordY = this->_calcPos(i) - 4; };
        void invalidatePos() { _posX = -1; _posY = -1; };
        bool validPos();

        virtual ~highlight();
    protected:
    private:
};

#endif // HIGHLIGHT_H
