#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "./layoutprops.hpp"
#include "./decoration.hpp"
#include "./vec2.hpp"

typedef struct
{
    Window win;
    Window frame;

    bool fullscreen;
    Vec2 fullscreen_revert_size;
    Vec2 fullscreen_revert_pos;

    u_int8_t monitor_index;
    int8_t desktop_index;

    LayoutProps layout;
    bool ignore_unmap, ignore_enter;

    Decoration decoration;
} Client;

#endif