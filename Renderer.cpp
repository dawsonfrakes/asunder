#if defined(USE_OPENGL)
    #include "opengl.hpp"

    #if defined(USE_XLIB)
        #include "PlatformRenderer_opengl_xlib.cpp"
    #endif

    // TODO: implement win32-open and USE_WIN32
    #if defined(USE_WIN32)
        #include "PlatformRenderer_opengl_win32.cpp"
    #endif
#endif

// TODO: implement vulkan and USE_VULKAN
#if defined(USE_VULKAN)
    #include "PlatformRenderer_vulkan.cpp"
#endif
