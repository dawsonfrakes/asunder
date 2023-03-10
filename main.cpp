#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>

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

struct WindowWin32 {
    HINSTANCE inst;
    HWND hwnd;
    HDC hdc;

    WindowWin32() {
        inst = GetModuleHandleA(nullptr);
        WNDCLASSA c {};
        c.style = CS_OWNDC;
        c.lpfnWndProc = [](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
            switch (msg) {
                case WM_DESTROY: { PostQuitMessage(0); } break;
                default: return DefWindowProcA(hwnd, msg, wp, lp);
            }
            return LRESULT(0);
        };
        c.hInstance = inst;
        c.lpszClassName = "A";
        RegisterClassA(&c);
        hwnd = CreateWindowExA(0,
            "A",
            "Title",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            nullptr,
            nullptr,
            inst,
            nullptr
        );
        hdc = GetDC(hwnd);
    }

    bool update() {
        MSG msg;
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE) > 0) {
            if (msg.message == WM_QUIT)
                return false;
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        return true;
    }
};

#define GL_FUNCTIONS \
    GL_FUNCTION(void, ClipControl, GLenum origin, GLenum depth)

namespace GL {
    constexpr auto COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT;
    constexpr auto UPPER_LEFT = 0x8CA2;
    constexpr auto ZERO_TO_ONE = 0x935F;

    const auto ClearColor = glClearColor;
    const auto Clear = glClear;
#define GL_FUNCTION(RET, NAME, ...) RET (*NAME)(__VA_ARGS__);
    GL_FUNCTIONS
#undef GL_FUNCTION
}

struct OpenGLWin32 {
    HGLRC context;
    const HDC &hdc;

    OpenGLWin32(const WindowWin32 &window) : hdc(window.hdc) {
        PIXELFORMATDESCRIPTOR pfd {};
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        SetPixelFormat(window.hdc, ChoosePixelFormat(window.hdc, &pfd), &pfd);

        const auto wglCreateContextAttribsARB = [&]() {
            const auto c = wglCreateContext(window.hdc);
            wglMakeCurrent(window.hdc, c);
            const auto result = wglGetProcAddress("wglCreateContextAttribsARB");
            wglDeleteContext(c);
            return (HGLRC (*)(HDC hDC, HGLRC hShareContext, const int *attribList)) result;
        }();
        constexpr auto WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091;
        constexpr auto WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092;
        constexpr auto WGL_CONTEXT_FLAGS_ARB = 0x2094;
        constexpr auto WGL_CONTEXT_PROFILE_MASK_ARB = 0x9126;
        constexpr auto WGL_CONTEXT_DEBUG_BIT_ARB = 0x0001;
        constexpr auto WGL_CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001;
        constexpr int params[] {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 5,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        context = wglCreateContextAttribsARB(window.hdc, nullptr, params);
        wglMakeCurrent(window.hdc, context);

        if (const auto wglSwapIntervalEXT = (BOOL (*)(int)) wglGetProcAddress("wglSwapIntervalEXT"))
            wglSwapIntervalEXT(0);

#define GL_FUNCTION(RET, NAME, ...) \
    GL::NAME = (RET(*)(__VA_ARGS__))wglGetProcAddress("gl" #NAME);
        GL_FUNCTIONS
#undef GL_FUNCTION

        GL::ClipControl(GL::UPPER_LEFT, GL::ZERO_TO_ONE);
    }

    void update() { SwapBuffers(hdc); }
};

int main() {
    WindowWin32 window;
    OpenGLWin32 renderer(window);
    while (true) {
        if (!window.update())
            break;
        GL::ClearColor(1.0, 0.0, 1.0, 1.0);
        GL::Clear(GL::COLOR_BUFFER_BIT);
        renderer.update();
    }
}
