#ifndef SCRATCHPAD_DEF_HPP
#define SCRATCHPAD_DEF_HPP

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

typedef struct {
    const char* cmd;
    u_int8_t key;
    bool spawned, hidden;
    Window win, frame;
} ScratchpadDef;

#endif