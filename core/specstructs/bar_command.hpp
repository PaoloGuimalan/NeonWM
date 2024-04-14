#ifndef BAR_COMMAND_HPP
#define BAR_COMMAND_HPP

#include <X11/keysym.h> // Include necessary headers for XK_1, XK_2, etc.
#include <X11/Xft/Xft.h>

struct BarCommand {
    const char* cmd;
    const char* color;
    int32_t bg_color;
    XftColor _xcolor;
    char _text[512];
};

#endif // BAR_COMMAND_HPP