#ifndef NEONWM_HPP
#define NEONWM_HPP

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include "../types/config.hpp"
#include "../types/enums.hpp"
#include "./client.hpp"
#include "./vec2.hpp"
#include "./bar.hpp"

#define CLIENT_WINDOW_CAP 256

typedef struct
{
    Display* display;
    Window root;
    int screen;

    bool running;

    Client client_windows[CLIENT_WINDOW_CAP];
    u_int32_t clients_count;

    int8_t focused_monitor;
    int32_t bar_monitor;
    int32_t focused_client;
    int8_t focused_desktop[MONITOR_COUNT];
    int32_t hard_focused_window_index;

    Vec2 cursor_start_pos, cursor_start_frame_pos, cursor_start_frame_size;

    WindowLayout current_layout;
    u_int32_t layout_master_size[MONITOR_COUNT][DESKTOP_COUNT];
    bool layout_full;
    int32_t window_gap;

    bool decoration_hidden;
    Bar bar;
    double bar_refresh_timer;

    bool spawning_scratchpad;
    int32_t spawned_scratchpad_index;
} NeonWM;

#endif