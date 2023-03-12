#include <GL/gl.h>

namespace OpenGL {
#define GL_FUNCTIONS \
    GL_FUNCTION(void, ClipControl, GLenum origin, GLenum depth) \
    GL_FUNCTION(void, Clear, GLbitfield mask) \
    GL_FUNCTION(void, ClearColor, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) \
    GL_FUNCTION(void, DepthRange, GLdouble nearVal, GLdouble farVal)

    constexpr auto COLOR_BUFFER_BIT = 0x00004000;
    constexpr auto LOWER_LEFT = 0x8CA1;
    constexpr auto UPPER_LEFT = 0x8CA2;
    constexpr auto ZERO_TO_ONE = 0x935F;

#define GL_FUNCTION(RET, NAME, ...) RET (*NAME)(__VA_ARGS__);
    GL_FUNCTIONS
#undef GL_FUNCTION
}
