#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <square.h>


class chessMan : public square
{
    public:
        chessMan(bool, int, int);

        void moveTo(int x, int y){ this->setPosX(x); this->setPosY(y); };
        const int getType() const { return _type; };
        const int getIndex(int) const;

        const int getID() const { return _id; };

        virtual ~chessMan();
    protected:
        const int _id, _type;
    private:
};

#endif // CHESSMAN_H
