#include "highlight.h"

highlight::highlight()
{
    _posX = -1; //ctor
    _posY = -1;
}

void highlight::setPos(int x, int y){
    _posX = x;
    _coordX = this->_calcPos(x) - 4;

    _posY = y;
    _coordY = this->_calcPos(y) - 4;
}

bool highlight::validPos() const{
    if (_posX < 0 && _posY < 0){
        return false;
    } else {
        return true;
    }
}

highlight::~highlight()
{
    //dtor
}
