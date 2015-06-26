#ifndef KNIGHT_H
#define KNIGHT_H

#include <chessMan.h>


class knight : public chessMan
{
    public:
        knight(bool, int);

        virtual ~knight();
    protected:
    private:
        knight();
};

#endif // KNIGHT_H
