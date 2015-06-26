#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <square.h>


class chessMan : public square
{
    public:
        chessMan();

        void moveTo(int x, int y){ this->setPosX(x); this->setPosY(y); };

        const int getID() const { return _id; };

        virtual ~chessMan();
    protected:
        int _id;
    private:
};

#endif // CHESSMAN_H
