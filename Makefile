# make => build dynamic gamemode loader (think Garry's Mod)
# make game GAME=./game.cpp => (re)build dynamic gamemode
# or
# make GAME=./game.cpp => build static engine+gamemode

# LDFLAGS = -lgdi32 -lopengl32 # Windows
LDFLAGS = -lX11 -lGL # Linux

CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g -DUSE_XLIB -DUSE_OPENGL
ifeq ($(origin GAME), undefined)
GAME = game_dynamic.cpp
# LDFLAGS += ??? # Windows
LDFLAGS += -ldl
DYNAMIC = true
endif

all:
	$(CXX) $(CXXFLAGS) main.cpp $(GAME) -o game $(LDFLAGS)

game:
ifneq ($(DYNAMIC), true)
	$(CXX) -shared -fpic $(CXXFLAGS) $(GAME) -o game.so
else
	@echo 'Error! usage: make $@ GAME=./path/to/game.cpp'
endif

.PHONY: all game
