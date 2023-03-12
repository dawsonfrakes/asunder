#if defined(USE_XLIB)
    #include "PlatformWindow_xlib.cpp"
#endif

#if defined(USE_WIN32)
    #include "PlatformWindow_win32.cpp"
#endif
