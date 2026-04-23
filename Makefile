# ============================================================
#  Makefile  –  alternative to CMake (Linux / macOS)
# ============================================================
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SFML     = -lsfml-graphics -lsfml-window -lsfml-system
SRC      = src/main.cpp src/Maze.cpp src/Player.cpp src/Game.cpp
OUT      = ManInTheMaze

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(SFML)

clean:
	rm -f $(OUT)

run: all
	./$(OUT)
