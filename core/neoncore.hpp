#include <X11/Xcursor/Xcursor.h>
#include <stdlib.h>
#include "types/definitions.hpp"
#include <unistd.h>

class neoncore
{
    private:
        /* data */

    public:

        static NeonWM wm;

        static void redraw_client_decoration(Client* client) {
            if(!SHOW_DECORATION || (!DECORATION_SHOW_CLOSE_ICON && !DECORATION_SHOW_MAXIMIZE_ICON)) return;
            u_int32_t x_offset = 0;
            if(DECORATION_SHOW_CLOSE_ICON) {
                x_offset += DECORATION_CLOSE_ICON_SIZE;
                XClearWindow(wm.display, client->decoration.closebutton);
                XGlyphInfo extents;
                XftTextExtents16(wm.display, client->decoration.closebutton_font.font, (FcChar16*)DECORATION_CLOSE_ICON, strlen(DECORATION_CLOSE_ICON), &extents);
                draw_str(DECORATION_CLOSE_ICON, client->decoration.closebutton_font, NULL,
                        (DECORATION_CLOSE_ICON_SIZE / 2.0f) - (extents.xOff / 2.0f), (DECORATION_TITLEBAR_SIZE / 2.0f) + (extents.height / 2.0f)); 
            }
            if(DECORATION_SHOW_MAXIMIZE_ICON) {
                x_offset += DECORATION_MAXIMIZE_ICON_SIZE;
                XClearWindow(wm.display, client->decoration.maximize_button);
                XGlyphInfo extents;
                XftTextExtents16(wm.display, client->decoration.maximize_button_font.font, (FcChar16*)DECORATION_MAXIMIZE_ICON, strlen(DECORATION_MAXIMIZE_ICON), &extents);
                draw_str(DECORATION_MAXIMIZE_ICON, client->decoration.maximize_button_font, NULL,
                        (DECORATION_MAXIMIZE_ICON_SIZE / 2.0f) - (extents.xOff / 2.0f), (DECORATION_TITLEBAR_SIZE / 2.0f) + (extents.height / 2.0f)); 
            }
            XClearWindow(wm.display, client->decoration.titlebar);
            char* window_name = NULL;
            XFetchName(wm.display, client->win, &window_name);
            if(window_name != NULL) {
                XGlyphInfo extents;
                XftTextExtents16(wm.display, client->decoration.titlebar_font.font, (FcChar16*)window_name, strlen(window_name), &extents);
                XSetForeground(wm.display, DefaultGC(wm.display, wm.screen), DECORATION_TITLE_COLOR);
                XFillRectangle(wm.display, client->decoration.titlebar, DefaultGC(wm.display, wm.screen), 0, 0, extents.xOff, DECORATION_TITLEBAR_SIZE);
                draw_str(window_name, client->decoration.titlebar_font, NULL, 0, (DECORATION_TITLEBAR_SIZE / 2.0f) + (extents.height / 2.0f));
                XFree(window_name);
                draw_design(client->decoration.titlebar, extents.xOff, DECORATION_TITLE_LABEL_DESIGN, DECORATION_TITLE_COLOR, DECORATION_DESIGN_WIDTH, DECORATION_TITLEBAR_SIZE);
            }

            XWindowAttributes attribs;
            XGetWindowAttributes(wm.display, client->frame, &attribs);
            draw_design(client->decoration.titlebar, attribs.width - x_offset, DECORATION_ICONS_LABEL_DESIGN, DECORATION_MAXIMIZE_ICON_COLOR, DECORATION_DESIGN_WIDTH, DECORATION_TITLEBAR_SIZE);
        }

        static void draw_half_circle(Window win, Vec2 pos, int32_t radius, u_int32_t color, bool to_left) {
            XSetForeground(wm.display, DefaultGC(wm.display, wm.screen), color);

            if(to_left) {
                XFillArc(wm.display, win,DefaultGC(wm.display, wm.screen), pos.x - radius, pos.y - radius, radius * 2, radius * 2, 90 * 64, 180 * 64);
            } else {
                XRectangle rect;
                rect.x = pos.x + radius;
                rect.y = pos.y;
                rect.width = radius;
                rect.height = radius * 2;
                Region clipRegion = XCreateRegion();
                XUnionRectWithRegion(&rect, clipRegion, clipRegion);
                XFillArc(wm.display, win,DefaultGC(wm.display, wm.screen), pos.x - BAR_LABEL_DESIGN_WIDTH - radius, pos.y - radius, radius * 2, radius * 2, 90  * 64, -180 * 64);
            }
        }

        static void draw_triangle(Window win, Vec2 p1, Vec2 p2, Vec2 p3, u_int32_t color) {
            XPoint points[3] = {
                {.x = static_cast<short>(p1.x), .y = static_cast<short>(p1.y)},
                {.x = static_cast<short>(p2.x), .y = static_cast<short>(p2.y)},
                {.x = static_cast<short>(p3.x), .y = static_cast<short>(p3.y)},
            };
            XSetForeground(wm.display, DefaultGC(wm.display, wm.screen), color);
            XFillPolygon(wm.display, win, DefaultGC(wm.display, wm.screen), points, 3, Convex, CoordModeOrigin);
        }

        static void draw_design(Window win, int32_t xpos, BarLabelDesign design, u_int32_t color, u_int32_t design_width, u_int32_t design_height) {
            switch (design) {
                case DESIGN_SHARP_RIGHT_UP: {
                    draw_triangle(win, 
                                (Vec2){.x = static_cast<float>(xpos), .y = 0}, 
                                (Vec2){.x = static_cast<float>(xpos) + design_width, .y = static_cast<float>(design_height)}, 
                                (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height)}, 
                                color);
                    break;
                }
                case DESIGN_SHARP_LEFT_UP: {
                    draw_triangle(win, 
                                (Vec2){.x = static_cast<float>(xpos), .y = 0}, 
                                (Vec2){.x = static_cast<float>(xpos) - design_width, .y = static_cast<float>(design_height)}, 
                                (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height)}, 
                                color);
                    break;
                }
                case DESIGN_SHARP_RIGHT_DOWN: {
                    draw_triangle(win, 
                                (Vec2){.x = static_cast<float>(xpos), .y = 0}, 
                                (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height)}, 
                                (Vec2){.x = static_cast<float>(xpos) + design_width, .y = 0}, 
                                color);
                    break;
                }
                case DESIGN_SHARP_LEFT_DOWN: {
                    draw_triangle(win, 
                                (Vec2){.x = static_cast<float>(xpos), .y = 0}, 
                                (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height)}, 
                                (Vec2){.x = static_cast<float>(xpos) - design_width, .y = 0}, 
                                color);
                    break;
                }
                case DESIGN_ARROW_LEFT: {
                    draw_triangle(win, 
                                (Vec2){.x = static_cast<float>(xpos), .y = 0}, 
                                (Vec2){.x = static_cast<float>(xpos) - design_width, .y = static_cast<float>(design_height) / 2.0f}, 
                                (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height) / 2.0f}, 
                                color);
                    draw_triangle(win, 
                                (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height) / 2.0f}, 
                                (Vec2){.x = static_cast<float>(xpos) - design_width, .y = static_cast<float>(design_height) / 2.0f}, 
                                (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height)}, 
                                color);
                    break;
                }
                case DESIGN_ARROW_RIGHT: {
                    draw_triangle(win, 
                                (Vec2){.x = static_cast<float>(xpos), .y = 0}, 
                                (Vec2){.x = static_cast<float>(xpos) + design_width, .y = static_cast<float>(design_height) / 2.0f}, 
                                (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height) / 2.0f}, 
                                color);
                    draw_triangle(win, 
                                (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height) / 2.0f}, 
                                (Vec2){.x = static_cast<float>(xpos) + design_width, .y = static_cast<float>(design_height) / 2.0f}, 
                                (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height)}, 
                                color);
                    break;
                }
                case DESIGN_ROUND_LEFT: {
                    draw_half_circle(win, (Vec2){.x = static_cast<float>(xpos), .y = static_cast<float>(design_height) / 2.0f}, static_cast<float>(design_height) / 2.0f, color,true);
                    break;
                }
                case DESIGN_ROUND_RIGHT: {
                    draw_half_circle(win, (Vec2){.x = static_cast<float>(xpos) + design_width, .y = static_cast<float>(design_height) / 2.0f}, static_cast<float>(design_height) / 2.0f, color,false);
                    break;
                }
                case DESIGN_STRAIGHT: {
                    XSetForeground(wm.display, DefaultGC(wm.display, wm.screen), color);
                    XFillRectangle(wm.display, win, DefaultGC(wm.display, wm.screen), xpos, 0, design_width, design_height);
                    break;
                } 
            }   
        }

        static void draw_str(const char* str, FontStruct font, XftColor* color, int x, int y) {
            XftDrawStringUtf8(font.draw, color ? color : &font.color, font.font, x, y, (XftChar8 *)str, strlen(str));
        }

        static bool str_unicode(const char* str) {
            while (*str != '\0') {
                if ((unsigned char)(*str) > 127) {
                    return true;
                }
                str++;
            }
            return false;
        }

        static void get_cmd_output(const char* cmd, char* dst){
            FILE* fp;
            char line[256];
            fp = popen(cmd, "r");
            while(fgets(line, sizeof(line), fp) != NULL){
                strcpy(dst, line);
            }
            dst[strlen(dst) - 1] = '\0';
            pclose(fp);
        }

        static int handle_x_error(Display* display, XErrorEvent* e){
            (void)display;
            char err_msg[1024];
            XGetErrorText(display, e->error_code, err_msg, sizeof(err_msg));
            printf("X Error:\n\tRequest: %i\n\tError Code: %i - %s\n\tResource ID: %i\n", e->request_code, e->error_code, err_msg, (int)e->resourceid);
            return 0;
        }

        static void draw_bar(){
            if(!SHOW_BAR || wm.bar.hidden || !wm.bar.init) return;

            XClearWindow(wm.display, wm.bar.win);
            u_int32_t xoffset = 0;
            {
                for(u_int32_t i = 0; i < BAR_COMMANDS_COUNT; i++){
                    get_cmd_output(BarCommands[i].cmd, BarCommands[i]._text);

                    char text[516] = {0};
                    memcpy(text, BarCommands[i]._text, sizeof(text));
                    if(strlen(BAR_COMMAND_SEPARATOR) != 0 && i != BAR_COMMANDS_COUNT - 1)
                        strcat(text, " " BAR_COMMAND_SEPARATOR);

                    int length = strlen(text);
                    for(int i = length; i >= 0; i--){
                        text[i + 1] = text[i];
                    }
                    text[0] = ' ';

                    int32_t xoff = 0;
                    bool is_unicode = false;
                    for(u_int32_t i = 0; i < strlen(text); i++) {
                        XGlyphInfo extents;
                        char str[2];
                        str[0] = text[i];
                        str[1] = '\0';
                        if(str_unicode(str)) {
                            XftTextExtentsUtf16(wm.display, wm.bar.font.font, (FcChar8*)str, FcEndianBig, strlen(str), &extents);
                            is_unicode = true;
                        } else {
                            XftTextExtents8(wm.display, wm.bar.font.font, (FcChar8*)str, strlen(str), &extents);
                        }
                        xoff += extents.xOff;
                    }
                    if(is_unicode) {
                        XGlyphInfo extents;
                        if(str_unicode(BAR_COMMAND_SEPARATOR)) {
                            XftTextExtentsUtf16(wm.display, wm.bar.font.font, (FcChar8*)BAR_COMMAND_SEPARATOR, FcEndianBig, strlen(BAR_COMMAND_SEPARATOR), &extents);
                        } else {
                            XftTextExtents8(wm.display, wm.bar.font.font, (FcChar8*)BAR_COMMAND_SEPARATOR, strlen(BAR_COMMAND_SEPARATOR), &extents);
                        }
                        xoff += extents.xOff;
                    }
                    XSetForeground(wm.display, DefaultGC(wm.display, wm.screen), BarCommands[i].bg_color == -1 ? BAR_MAIN_LABEL_COLOR : BarCommands[i].bg_color);
                    XFillRectangle(wm.display, wm.bar.win, DefaultGC(wm.display, wm.screen), xoffset, 0, xoff, BAR_SIZE);
                    
                    draw_str(text, wm.bar.font, &BarCommands[i]._xcolor, xoffset, (BAR_SIZE / 2.0f) + (FONT_SIZE / 2.0f));
                    xoffset += xoff;
                }
                draw_design(wm.bar.win, xoffset, BAR_MAIN_LABEL_DESIGN, BAR_MAIN_LABEL_COLOR, BAR_LABEL_DESIGN_WIDTH, BAR_SIZE);
                xoffset += BAR_LABEL_DESIGN_WIDTH;
            }
        }

        static void grab_global_input(){
            XGrabKey(wm.display, XKeysymToKeycode(wm.display, WM_TERMINATE_KEY), MASTER_KEY, wm.root, false, GrabModeAsync, GrabModeAsync);

            XGrabKey(wm.display, XKeysymToKeycode(wm.display, APPLICATION_LAUNCHER_OPEN_KEY), MASTER_KEY, wm.root, false, GrabModeAsync, GrabModeAsync);
            XGrabKey(wm.display, XKeysymToKeycode(wm.display, TERMINAL_OPEN_KEY), MASTER_KEY, wm.root, false, GrabModeAsync, GrabModeAsync);
            XGrabKey(wm.display, XKeysymToKeycode(wm.display, WEB_BROWSER_OPEN_KEY), MASTER_KEY, wm.root, false, GrabModeAsync, GrabModeAsync);
            
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, DESKTOP_CYCLE_UP_KEY),MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, DESKTOP_CYCLE_DOWN_KEY),MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);

            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_LAYOUT_TILED_MASTER_KEY),MASTER_KEY | ShiftMask,wm.root,false, GrabModeAsync,GrabModeAsync);
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_LAYOUT_FLOATING_KEY),MASTER_KEY | ShiftMask,wm.root,false, GrabModeAsync,GrabModeAsync);
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_LAYOUT_HORIZONTAL_MASTER_KEY),MASTER_KEY | ShiftMask,wm.root,false, GrabModeAsync,GrabModeAsync);
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_LAYOUT_HORIZONTAL_STRIPES_KEY),MASTER_KEY | ShiftMask,wm.root,false, GrabModeAsync,GrabModeAsync);
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_LAYOUT_VERTICAL_STRIPES_KEY),MASTER_KEY | ShiftMask,wm.root,false, GrabModeAsync,GrabModeAsync);

            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_CYCLE_KEY),MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_LAYOUT_DECREASE_MASTER_KEY),MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_LAYOUT_INCREASE_MASTER_KEY),MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_LAYOUT_INCREASE_SLAVE_KEY),MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_LAYOUT_DECREASE_SLAVE_KEY),MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
            XGrabKey(wm.display,XKeysymToKeycode(wm.display, WINDOW_LAYOUT_SET_MASTER_KEY),MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
            if(SHOW_BAR) {
                XGrabKey(wm.display,XKeysymToKeycode(wm.display, BAR_TOGGLE_KEY), MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
                if(MONITOR_COUNT > 1) {
                    XGrabKey(wm.display,XKeysymToKeycode(wm.display, BAR_CYCLE_MONITOR_UP_KEY),MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
                    XGrabKey(wm.display,XKeysymToKeycode(wm.display, BAR_CYCLE_MONITOR_DOWN_KEY),MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
                }
            }
            
            if(SHOW_DECORATION) {
                XGrabKey(wm.display,XKeysymToKeycode(wm.display, DECORATION_TOGGLE_KEY), MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
            }
            for(u_int32_t i = 0; i < SCRATCH_PAD_COUNT; i++) {
                XGrabKey(wm.display,XKeysymToKeycode(wm.display, ScratchpadDefs[i].key), MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
            }
            for(u_int32_t i = 0; i < CUSTOM_KEYBIND_COUNT; i++) {
                XGrabKey(wm.display,XKeysymToKeycode(wm.display, CustomKeybinds[i].key), MASTER_KEY,wm.root,false, GrabModeAsync,GrabModeAsync);
            }
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

        static void* update_ui_thread(void* arg) {
            neoncore* instance = static_cast<neoncore*>(arg);
            return instance->update_ui();
        }

        void* update_ui(){
            while(true){
                usleep(UI_REFRESH_RATE * 1000000);
                XFlush(wm.display);
                draw_bar();
                if(SHOW_DECORATION){
                    for(u_int32_t i = 0; i < wm.clients_count; i++){
                        if(wm.client_windows[i].desktop_index == wm.focused_desktop[wm.focused_monitor]){
                            redraw_client_decoration(&wm.client_windows[i]);
                        }
                    }
                }
            }
            return NULL;
        }

        int32_t get_client_index_window(Window win) {
            for(u_int32_t i = 0; i < wm.clients_count; i++) {
                if(wm.client_windows[i].win == win || wm.client_windows[i].frame == win) return i;
            }
            return -1;
        }

        int32_t get_scratchpad_index_window(Window win) {
            for(u_int32_t i = 0; i < SCRATCH_PAD_COUNT; i++) {
                if(ScratchpadDefs[i].win == win) return i;
            }
            return -1;
        }

        void neonwm_terminate() {
            XUnmapWindow(wm.display, wm.bar.win);

            // Free all the allocated colors 
            for(u_int32_t i = 0; i < BAR_COMMANDS_COUNT; i++) {
                XftColorFree(wm.display, DefaultVisual(wm.display, wm.screen), DefaultColormap(wm.display, wm.screen), &BarCommands[i]._xcolor); 
            }
            for(u_int32_t i = 0; i < DESKTOP_COUNT; i++) {
                XftColorFree(wm.display, DefaultVisual(wm.display, wm.screen), DefaultColormap(wm.display, wm.screen), &DesktopIcons[i]._xcolor); 
            }
            XCloseDisplay(wm.display);
        }

        FontStruct font_create(const char* fontname, const char* fontcolor, Window win) {
            FontStruct fs;
            XftFont* xft_font = XftFontOpenName(wm.display, wm.screen, fontname);
            XftDraw* xft_draw = XftDrawCreate(wm.display, win, DefaultVisual(wm.display, wm.screen), DefaultColormap(wm.display, wm.screen)); 
            XftColor xft_font_color;
            XftColorAllocName(wm.display,DefaultVisual(wm.display,0),DefaultColormap(wm.display,0),fontcolor, &xft_font_color);

            fs.font = xft_font;
            fs.draw = xft_draw;
            fs.color = xft_font_color;

            return fs;
        }

        u_int32_t draw_text_icon_color(const char* icon, const char* text, Vec2 pos, FontStruct font, u_int32_t color, u_int32_t rect_x_add, Window win, u_int32_t height) {
            XGlyphInfo extents;
            XftTextExtents16(wm.display, font.font, (FcChar16*)text, strlen(text), &extents);
            XGlyphInfo extents_icon;
            XftTextExtents16(wm.display, font.font, (FcChar16*)icon, strlen(icon), &extents_icon);

            XSetForeground(wm.display, DefaultGC(wm.display, wm.screen), color);
            XFillRectangle(wm.display, win, DefaultGC(wm.display, wm.screen), pos.x, 0, extents.xOff + extents_icon.xOff + rect_x_add, height);

            draw_str(icon, font, NULL, pos.x, pos.y);
            draw_str(text, font, NULL, pos.x + extents_icon.xOff, pos.y);

            return extents.xOff + extents_icon.xOff + rect_x_add;
        }

        void draw_bar_buttons() {
            if(!SHOW_BAR || wm.bar.hidden) return;
            u_int32_t xoffset = 0;
            for(u_int32_t i = 0; i < BAR_BUTTON_COUNT; i++) {
                XRaiseWindow(wm.display, BarButtons[i].win);
                XClearWindow(wm.display, BarButtons[i].win);
                XGlyphInfo extents;
                if(str_unicode(BarButtons[i].icon)) {
                    XftTextExtentsUtf16(wm.display, wm.bar.font.font, (FcChar8*)BarButtons[i].icon, FcEndianBig, strlen(BarButtons[i].icon), &extents);
                } else {
                    XftTextExtents8(wm.display, wm.bar.font.font, (FcChar8*)BarButtons[i].icon, strlen(BarButtons[i].icon), &extents);
                }

                BarButtons[i].font = font_create(FONT, FONT_COLOR, BarButtons[i].win);
                draw_text_icon_color(BarButtons[i].icon, "", (Vec2){(BAR_BUTTON_SIZE / 2.0f) - ((str_unicode(BarButtons[i].icon)) ? (extents.width) : (extents.width / 2.0f)), (BAR_SIZE / 2.0f) + (FONT_SIZE / 2.0f)}, BarButtons[i].font, BarButtons[i].color, 0, BarButtons[i].win, BAR_SIZE);
                xoffset += BAR_BUTTON_SIZE + BAR_BUTTON_PADDING;
            }
        }

        void raise_bar() {
            if(SHOW_BAR) {
                XRaiseWindow(wm.display, wm.bar.win);
                for(u_int32_t i = 0; i < BAR_BUTTON_COUNT; i++) {
                    XRaiseWindow(wm.display, BarButtons[i].win);
                }
            }
        }

        void unhide_bar() {
            if(!SHOW_BAR) return;
            if(!wm.bar.hidden) return;
            XMapWindow(wm.display, wm.bar.win);
            for(u_int32_t i = 0; i < BAR_BUTTON_COUNT; i++) {
                XMapWindow(wm.display, BarButtons[i].win);
            }
            raise_bar();
            wm.bar.hidden = false;
            draw_bar_buttons();
            draw_bar();
        }

        void neon_window_unframe(Window win) {
            int32_t client_index = get_client_index_window(win);
            if(client_index == -1) {
                return;
            }
            bool fullscreen = wm.client_windows[client_index].fullscreen;

            if(wm.client_windows[client_index].ignore_unmap) {
                wm.client_windows[client_index].ignore_unmap = false;
                return;
            }
            if(wm.client_windows[client_index].fullscreen) {
                unhide_bar();
            }
            Window frame_win = wm.client_windows[client_index].frame;

            if(wm.client_windows[client_index].layout.in && get_client_index_window(win) == 0) {
                wm.layout_master_size[wm.focused_monitor][wm.focused_desktop[wm.focused_monitor]] = 0;
            }
            if(SHOW_DECORATION) {
                XftColorFree(wm.display, DefaultVisual(wm.display, 0), DefaultColormap(wm.display, wm.screen), &wm.client_windows[client_index].decoration.titlebar_font.color);
                XftFontClose(wm.display, wm.client_windows[client_index].decoration.titlebar_font.font);

                XftColorFree(wm.display, DefaultVisual(wm.display, 0), DefaultColormap(wm.display, wm.screen), &wm.client_windows[client_index].decoration.titlebar_font.color);
                XftFontClose(wm.display, wm.client_windows[client_index].decoration.titlebar_font.font);
            }

            XReparentWindow(wm.display, frame_win, wm.root, 0, 0);
            XUnmapWindow(wm.display, frame_win);
            XSetInputFocus(wm.display, wm.root, RevertToPointerRoot, CurrentTime);

            for(u_int32_t i = client_index; i < wm.clients_count - 1; i++) {
                wm.client_windows[i] = wm.client_windows[i + 1];
            }
            memset(&wm.client_windows[wm.clients_count - 1], 0, sizeof(Client));
            wm.clients_count--;

            if(wm.clients_count != 0) {
                wm.client_windows[wm.clients_count - 1].layout.change = 0;
            }
            if(wm.hard_focused_window_index == client_index) {
                wm.hard_focused_window_index = -1;
            }
            draw_bar_buttons();
            draw_bar();
            if(fullscreen) {
                unhide_bar();
            }
            // establish_window_layout();
        }

        void handle_unmap_notify(XUnmapEvent e) {
            if(get_client_index_window(e.window) != -1) {
                neon_window_unframe(e.window);
                return;
            }
            if(get_scratchpad_index_window(e.window) != -1) {
                XUnmapWindow(wm.display, ScratchpadDefs[get_scratchpad_index_window(e.window)].frame);
                ScratchpadDefs[get_scratchpad_index_window(e.window)].spawned = false;
                XSetInputFocus(wm.display, wm.root, RevertToPointerRoot, CurrentTime);
            }
        }

        void neonwm_run() {
            XEvent e;
            while(wm.running) {
                XNextEvent(wm.display, &e);
                switch (e.type) {
                    case UnmapNotify:
                        handle_unmap_notify(e.xunmap);
                        break;
                    case MapRequest:
                        // handle_map_request(e.xmaprequest);
                        break;
                    case ConfigureRequest:
                        // handle_configure_request(e.xconfigurerequest);
                        break;
                    case ButtonPress:
                        // handle_button_press(e.xbutton);
                        break;
                    case KeyPress:
                        // handle_key_press(e.xkey);
                        break;
                    case MotionNotify: {
                        // select_focused_monitor(get_cursor_position().x);
                        // handle_motion_notify(e.xmotion);
                        break;
                    }
                    case EnterNotify: {
                        if(!WINDOW_SELECT_HOVERED || wm.hard_focused_window_index != -1) break;
                        int32_t client_index = get_client_index_window(e.xcrossing.window);
                        if(client_index == -1) break;
                        if(wm.client_windows[client_index].ignore_enter) {
                            wm.client_windows[client_index].ignore_enter = false;
                            break;
                        }
                        wm.focused_client = client_index;
                        for(u_int32_t i = 0; i < wm.clients_count; i++) {
                            if(i == (u_int32_t)client_index) {
                                XSetInputFocus(wm.display, wm.client_windows[i].win, RevertToPointerRoot, CurrentTime);
                                XSetWindowBorder(wm.display, wm.client_windows[i].frame, WINDOW_BORDER_COLOR_ACTIVE);
                            } else {
                                XSetWindowBorder(wm.display, wm.client_windows[i].frame, WINDOW_BORDER_COLOR);
                            }
                        }
                        break;
                    }
                }
            }

            neonwm_terminate();
        }
};

NeonWM neoncore::wm;
