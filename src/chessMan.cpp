#include "chessMan.h"

chessMan::chessMan(bool isWhite, int type, int id):
    _id(id),
    _type(type)
{
    _isWhite = isWhite; //ctor
}

//_____________Set visibility of chessMan____________
void chessMan::setVisible(bool i){
    _isVisible = i;
    if (!i) this->setPos(-1, -1);   // If visibility is set to false, invalidate position
}

//_____________Get index of chessMan_______________
const int chessMan::getIndex(int i) const{
    switch (i){
        case 111: return 0;     // WHITE PAWNs
        case 112: return 1;
        case 113: return 2;
        case 114: return 3;
        case 115: return 4;
        case 116: return 5;
        case 117: return 6;
        case 118: return 7;

        case 121: return 16;    // WHITE KNIGHTs
        case 122: return 17;
        case 123: return 18;
        case 124: return 19;
        case 125: return 20;
        case 126: return 21;
        case 127: return 22;
        case 128: return 23;

        case 211: return 8;     // BLACK PAWNs
        case 212: return 9;
        case 213: return 10;
        case 214: return 11;
        case 215: return 12;
        case 216: return 13;
        case 217: return 14;
        case 218: return 15;

        case 221: return 24;    // BLACK KNIGHTs
        case 222: return 25;
        case 223: return 26;
        case 224: return 27;
        case 225: return 28;
        case 226: return 29;
        case 227: return 30;
        case 228: return 31;
        default:  return -1;
    }
}

chessMan::~chessMan()
{
    puts("Destructor call for chessMan Object\n"); //dtor
}
