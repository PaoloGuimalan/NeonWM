#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <X11/Xlib.h>

typedef struct {
    u_int32_t width, height;
} Monitor;

#endif