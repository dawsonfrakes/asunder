#include <cstdint>
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intptr_t isize;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t usize;

typedef float f32;
typedef double f64;

namespace GL {
#define GL_FUNCTIONS \
    GL_FUNCTION(void, ClipControl, GLenum origin, GLenum depth) \
    GL_FUNCTION(void, Clear, GLbitfield mask) \
    GL_FUNCTION(void, ClearColor, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)

    typedef u32 GLenum;
    typedef u32 GLbitfield;
    typedef f32 GLclampf;

    constexpr auto COLOR_BUFFER_BIT = 0x00004000;
    constexpr auto UPPER_LEFT = 0x8CA2;
    constexpr auto ZERO_TO_ONE = 0x935F;

#define GL_FUNCTION(RET, NAME, ...) RET (*NAME)(__VA_ARGS__);
    GL_FUNCTIONS
#undef GL_FUNCTION
}

#if defined(_WIN32)
#include "win32.cpp"
#elif defined(__linux__)
#include "xlib.cpp"
#endif

int main() {
    PlatformWindow window;
    PlatformOpenGL renderer(window);
    GL::ClipControl(GL::UPPER_LEFT, GL::ZERO_TO_ONE);
    while (true) {
        if (!window.update())
            break;
        GL::ClearColor(1.0, 0.0, 1.0, 1.0);
        GL::Clear(GL::COLOR_BUFFER_BIT);
        renderer.update();
    }
}
