#include "highlight.h"

highlight::highlight()
{
    _posX = -1; //ctor
    _posY = -1;
}

//____________Set position of highlight____________
void highlight::setPos(int x, int y){
    _posX = x;
    _coordX = this->_calcPos(x) - 4;

    _posY = y;
    _coordY = this->_calcPos(y) - 4;
}

//____________Check if highlight has a valid position____________
bool highlight::validPos() const{
    if (_posX < 0 && _posY < 0){
        return false;
    } else {
        return true;
    }
}

highlight::~highlight()
{
    puts("Destructor call for highlight Object\n"); //dtor
}
