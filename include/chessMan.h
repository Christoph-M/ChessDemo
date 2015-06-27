#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <square.h>


class chessMan : public square
{
    public:
        chessMan(bool, int, int);

        void moveTo(int x, int y){ this->setPos(x, y); };

        void setVisible(bool);

        const int getType()     const { return _type; };
        const int getID()       const { return _id;   };
        const int getIndex(int) const;

        virtual ~chessMan();
    protected:
        const int _id, _type;
    private:
};

#endif // CHESSMAN_H
