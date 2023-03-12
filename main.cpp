#include "globals.hpp"

#if defined(_WIN32)
#include "win32.cpp"
#elif defined(__linux__)
#include "xlib.cpp"
#endif

#include "game.hpp"

auto main() -> int {
    init();
    PlatformWindow window;
    PlatformOpenGL renderer(window);
    OpenGL::ClipControl(OpenGL::UPPER_LEFT, OpenGL::ZERO_TO_ONE);
    OpenGL::DepthRange(1.0, 0.0);
    while (true) {
        if (!window.update())
            break;
        OpenGL::ClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        OpenGL::Clear(OpenGL::COLOR_BUFFER_BIT);
        renderer.update();
    }
}
