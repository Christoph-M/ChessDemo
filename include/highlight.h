#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include <square.h>


class highlight : public square
{
    public:
        highlight();

        void setPos(int, int);
        void invalidatePos() { _posX = -1; _posY = -1; };
        bool validPos() const;

        virtual ~highlight();
    protected:
    private:
};

#endif // HIGHLIGHT_H
