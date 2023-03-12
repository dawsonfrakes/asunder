#include "globals.hpp"

#include "Window.cpp"
#include "Renderer.cpp"
#include "game.hpp"

auto main() -> int {
    init();
    Ugly::Window window;
    Ugly::Renderer renderer(window);
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
