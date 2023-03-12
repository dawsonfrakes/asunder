#include <GL/glx.h>

struct PlatformRenderer {
    GLXContext context;
    const Ugly::Window &w;

    PlatformRenderer(const Ugly::Window &window) : w(window) {}
};

#include "Renderer.h"

Ugly::Renderer::Renderer(const Ugly::Window &window) : PlatformRenderer(window) {
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
#define GL_FUNCTION(RET, NAME, ...) OpenGL::NAME = (RET (*)(__VA_ARGS__)) glXGetProcAddress((const GLubyte *) ("gl" #NAME));
    GL_FUNCTIONS
#undef GL_FUNCTION
}

void Ugly::Renderer::update() { glXSwapBuffers(w.dpy, w.win); }
