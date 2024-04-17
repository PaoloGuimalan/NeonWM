#ifndef DECORATION_HPP
#define DECORATION_HPP

#include "../types/enums.hpp"
#include "./fontstruct.hpp"
#include <X11/X.h>

typedef struct{
    bool hidden;
    Window titlebar;
    Window closebutton;
    Window maximize_button;
    FontStruct closebutton_font, titlebar_font, maximize_button_font;
} Decoration;

#endif