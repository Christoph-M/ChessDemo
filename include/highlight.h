#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include <square.h>


class highlight : public square
{
    public:
        highlight();

        void setPosX(int i)  { _posX = i; _coordX = (i * 64) + 20 - 4; };
        void setPosY(int i)  { _posY = i; _coordY = (i * 64) + 20 - 4; };
        void invalidatePos() { this->setPosX(-1); this->setPosY(-1); };
        bool validPos();

        virtual ~highlight();
    protected:
    private:
};

#endif // HIGHLIGHT_H
