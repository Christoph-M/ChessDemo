#ifndef PAWN_H
#define PAWN_H

#include <chessMan.h>


class pawn : public chessMan
{
    public:
        pawn(bool, int);
        virtual ~pawn();
    protected:
    private:
        pawn();
};

#endif // PAWN_H
