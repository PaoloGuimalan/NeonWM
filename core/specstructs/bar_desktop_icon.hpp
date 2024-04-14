#ifndef BAR_DESKTOP_ICON_HPP
#define BAR_DESKTOP_ICON_HPP

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

typedef struct {
    const char* icon;
    const char* color;
    XftColor _xcolor;
} BarDesktopIcon;

#endif