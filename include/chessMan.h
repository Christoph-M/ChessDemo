#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <square.h>


class chessMan : public square
{
    public:
        chessMan();

        const int getID() const { return _id; };

        virtual ~chessMan();
    protected:
        int _id;
    private:
};

#endif // CHESSMAN_H
