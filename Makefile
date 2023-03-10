SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = game
CXX = clang++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -g

# Windows
LDFLAGS = -lgdi32 -lopengl32

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

.cpp.o:
	$(CXX) -c $(CFLAGS) $<

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)
