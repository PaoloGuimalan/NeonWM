#ifndef KEYBIND_HPP
#define KEYBIND_HPP

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

typedef struct {
    const char* cmd; 
    u_int32_t key;
} Keybind;

#endif