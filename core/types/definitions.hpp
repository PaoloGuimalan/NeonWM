#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include "config.hpp"
#include "enums.hpp"

#define CLIENT_WINDOW_CAP 256

typedef struct{
    XftFont* font;
    XftColor color;
    XftDraw* draw;
} FontStruct;

typedef struct{
    bool hidden;
    Window titlebar;
    Window closebutton;
    Window maximize_button;
    FontStruct closebutton_font, titlebar_font, maximize_button_font;
} Decoration;

typedef struct{
    bool in, skip;
    int32_t change;
} LayoutProps;

typedef struct{
    float x, y;
} Vec2;

typedef struct
{
    Window win;
    Window frame;

    bool fullscreen;
    Vec2 fullscreen_revert_size;
    Vec2 fullscreen_revert_pos;

    u_int8_t monitor_index;
    int8_t desktop_index;

    LayoutProps current_layout;
    bool ignore_unmap, ignore_enter;

    Decoration decoration;
} Client;

typedef struct{
    Window win;
    bool hidden;
    FontStruct font;
    int32_t DESKTOP_LABEL_x;
    bool init;
} Bar;

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
