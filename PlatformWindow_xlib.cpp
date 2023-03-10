#include <X11/Xlib.h>

struct PlatformWindow {
    Display *dpy;
    Window win;
    Atom wm_close;
};

#include "Window.h"

#include <stdio.h>
Ugly::Window::Window() {
    dpy = XOpenDisplay(nullptr);
    const auto scr = DefaultScreen(dpy);
    const auto root = RootWindow(dpy, scr);
    XSetWindowAttributes attrs {};
    attrs.event_mask = ExposureMask;
    win = XCreateWindow(
        dpy,
        root,
        0, 0,
        800, 600,
        0,
        CopyFromParent,
        InputOutput,
        CopyFromParent,
        CWEventMask,
        &attrs
    );
    XStoreName(dpy, win, "Title");
    wm_close = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, win, &wm_close, 1);
    XMapWindow(dpy, win);
}

bool Ugly::Window::update() {
    while (XPending(dpy) > 0) {
        XEvent ev;
        XNextEvent(dpy, &ev);
        switch (ev.type) {
            case Expose: {
                printf("%d %d\n", ev.xexpose.width, ev.xexpose.height);
            } break;
            case ClientMessage: {
                if ((Atom) ev.xclient.data.l[0] == wm_close)
                    return false;
            } break;
            case DestroyNotify: return false;
        }
    }
    return true;
}
