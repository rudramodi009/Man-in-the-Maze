#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include<SFML/Graphics.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    int rows     = (argc > 1) ? atoi(argv[1]) : 15;
    int cols     = (argc > 2) ? atoi(argv[2]) : 15;
    int cellSize = (argc > 3) ? atoi(argv[3]) : 40;

    unsigned seed = (argc > 4)
                        ? atoi(argv[4])
                        : static_cast<unsigned>(time(nullptr));

    // Clamp to sane values
    rows     = max(5,  min(rows,     40));
    cols     = max(5,  min(cols,     40));
    cellSize = max(20, min(cellSize, 80));

    cout << "Man in the Maze\n"
         << "  Grid   : " << rows << " x " << cols << "\n"
         << "  Cell   : " << cellSize << " px\n"
         << "  Seed   : " << seed << "\n"
         << "  Controls: W/A/S/D or Arrow keys\n";

    Game game(rows, cols, cellSize, seed);
    game.run();

    return 0;
}