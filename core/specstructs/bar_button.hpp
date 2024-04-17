#ifndef BAR_BUTTON_HPP
#define BAR_BUTTON_HPP

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include "./fontstruct.hpp"

typedef struct
{
    const char* cmd;
    const char* icon;
    Window win;
    FontStruct font;
    u_int32_t color;
} BarButton;

#endif