#ifndef FONTSTRUCT_HPP
#define FONTSTRUCT_HPP

#include <X11/Xft/Xft.h>

typedef struct{
    XftFont* font;
    XftColor color;
    XftDraw* draw;
} FontStruct;

#endif