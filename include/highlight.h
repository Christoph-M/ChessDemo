#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include <square.h>


class highlight : public square
{
    public:
        highlight();

        void drawBitmap(HDC&, HDC&, HBITMAP&, HBITMAP&) const;

        virtual ~highlight();
    protected:
    private:
};

#endif // HIGHLIGHT_H
