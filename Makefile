SRC = main.cpp
# Windows
# LDFLAGS = -lgdi32 -lopengl32

# Linux (xlib)
LDFLAGS = -lX11 -lGL

OBJ = $(SRC:.cpp=.o)
TARGET = game
CXX = clang++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -g

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ): win32.cpp xlib.cpp

.cpp.o:
	$(CXX) -c $(CFLAGS) $<

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)
