#include "highlight.h"

highlight::highlight()
{
    _posX = -1; //ctor
    _posY = -1;
}

bool highlight::validPos(){
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
