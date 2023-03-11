#include "game.hpp"
#include <dlfcn.h>

static void *dll;
static void (*game_init)();
static void (*game_update)();
static void (*game_deinit)();

void init() {
    if (dll) {
        dlclose(dll);
    }
    dll = dlopen("./game.so", RTLD_NOW);
    game_init =  (void (*)()) dlsym(dll, "init");
    game_update = (void (*)()) dlsym(dll, "update");
    game_deinit = (void (*)()) dlsym(dll, "deinit");

    game_init();
}

void update() { game_update(); }
void deinit() { game_deinit(); }
