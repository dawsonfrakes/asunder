#include <X11/Xlib.h>
#include <GL/glx.h>
#include <stdio.h>

struct PlatformWindow {
    Display *dpy;
    Window win;
    Atom wm_close;

    PlatformWindow() {
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

    bool update() {
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
};

struct PlatformOpenGL {
    GLXContext context;
    const PlatformWindow &w;

    PlatformOpenGL(const PlatformWindow &window) : w(window) {
        const GLint attribs[] = {
            GLX_X_RENDERABLE, True,
            GLX_RENDER_TYPE, GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
            GLX_RED_SIZE, 8,
            GLX_GREEN_SIZE, 8,
            GLX_BLUE_SIZE, 8,
            GLX_ALPHA_SIZE, 8,
            GLX_DEPTH_SIZE, 24,
            GLX_STENCIL_SIZE, 8,
            GLX_DOUBLEBUFFER, True,
            None
        };
        int count;
        GLXFBConfig *fbcs = glXChooseFBConfig(window.dpy, DefaultScreen(window.dpy), attribs, &count);
        GLXFBConfig fbc = fbcs[0];
        XFree(fbcs);

        const int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 5,
            GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None

        };
        const auto glXCreateContextAttribsARB = (GLXContext (*)(Display *, GLXFBConfig, GLXContext, Bool, const int *)) glXGetProcAddress((const GLubyte *) "glXCreateContextAttribsARB");
        context = glXCreateContextAttribsARB(window.dpy, fbc, nullptr, True, context_attribs);
        glXMakeCurrent(window.dpy, window.win, context);
#define GL_FUNCTION(RET, NAME, ...) GL::NAME = (RET (*)(__VA_ARGS__)) glXGetProcAddress((const GLubyte *) ("gl" #NAME));
        GL_FUNCTIONS
#undef GL_FUNCTION
    }

    void update() { glXSwapBuffers(w.dpy, w.win); }
};
