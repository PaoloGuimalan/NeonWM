#include <X11/Xcursor/Xcursor.h>
#include <stdlib.h>
#include "types/definitions.hpp"

class neoncore
{
    private:
        /* data */

    public:

        static NeonWM wm;

        static int handle_x_error(Display* display, XErrorEvent* e){
            (void)display;
            char err_msg[1024];
            XGetErrorText(display, e->error_code, err_msg, sizeof(err_msg));
            printf("X Error:\n\tRequest: %i\n\tError Code: %i - %s\n\tResource ID: %i\n", e->request_code, e->error_code, err_msg, (int)e->resourceid);
            return 0;
        }

        static void grab_global_input(){
            XGrabKey(wm.display, XKeysymToKeycode(wm.display, WM_TERMINATE_KEY), MASTER_KEY, wm.root, false, GrabModeAsync, GrabModeAsync);
        }     

        void init_neon(){
            system("neonstart");
            wm.display = XOpenDisplay(NULL);
            if(!wm.display){
                printf("Error X Display");
            }

            wm.clients_count = 0;
            wm.cursor_start_frame_size = (Vec2){ .x = 0.0f, .y = 0.0f };
            wm.cursor_start_frame_pos = (Vec2){ .x = 0.0f, .y = 0.0f };
            wm.cursor_start_pos = (Vec2){ .x = 0.0f, .y = 0.0f };
            wm.running = true;
            wm.focused_monitor = MONITOR_COUNT - 1;
            wm.current_layout = WINDOW_LAYOUT_DEFAULT;
            wm.bar_monitor = BAR_START_MONITOR;
            wm.window_gap = WINDOW_INITIAL_GAP;
            wm.decoration_hidden = !SHOW_DECORATION;
            wm.spawning_scratchpad = false;
            wm.layout_full = false;
            wm.spawned_scratchpad_index = 0;
            wm.hard_focused_window_index = -1;
            wm.focused_client = -1;
            memset(wm.focused_desktop, 0, sizeof(wm.focused_desktop));
            memset(wm.layout_master_size, 0, sizeof(wm.layout_master_size));
            wm.root = DefaultRootWindow(wm.display);
            wm.screen = DefaultScreen(wm.display);

            const char *wmname = "NeonOS";
            XChangeProperty(wm.display, wm.root, XInternAtom(wm.display, "_NET_WM_NAME", False), XInternAtom(wm.display, "UTF8_STRING", False), 8, PropModeReplace, (const unsigned char *)wmname, strlen(wmname));
            XFlush(wm.display);
            XSelectInput(wm.display, wm.root, SubstructureRedirectMask | SubstructureNotifyMask);
            XSync(wm.display, false);

            Cursor cursor = XcursorLibraryLoadCursor(wm.display, "arrow");
            XDefineCursor(wm.display, wm.root, cursor);
            XSetErrorHandler(handle_x_error);

            XSetWindowAttributes attributes;
            attributes.event_mask = ButtonPress | KeyPress | SubstructureRedirectMask | SubstructureNotifyMask | PropertyChangeMask;
            XChangeWindowAttributes(wm.display, wm.root, CWEventMask, &attributes);

            grab_global_input();
        } 
};

NeonWM neoncore::wm;
