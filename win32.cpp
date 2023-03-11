#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct PlatformWindow {
    HINSTANCE inst;
    HWND hwnd;
    HDC hdc;

    PlatformWindow() {
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

#include "opengl.hpp"

struct PlatformOpenGL {
    HGLRC context;
    const HDC &hdc;

    PlatformOpenGL(const PlatformWindow &window) : hdc(window.hdc) {
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

        const auto opengl32 = GetModuleHandleA("opengl32");
#define GL_FUNCTION(RET, NAME, ...) if (!(OpenGL::NAME = (RET (*)(__VA_ARGS__))wglGetProcAddress("gl" #NAME))) OpenGL::NAME = (RET (*)(__VA_ARGS__)) GetProcAddress(opengl32, "gl" #NAME);
        GL_FUNCTIONS
#undef GL_FUNCTION
    }

    void update() { SwapBuffers(hdc); }
};
