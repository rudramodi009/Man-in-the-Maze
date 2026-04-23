// ============================================================
//  Maze.cpp  –  DFS maze generation implementation
// ============================================================
//
//  HOW DFS MAZE GENERATION WORKS (step by step)
//  ─────────────────────────────────────────────
//  1. Start at cell (0,0), mark it visited.
//  2. Collect its unvisited neighbours in a shuffled list.
//  3. For each neighbour:
//       a. Remove the wall between current cell and neighbour.
//       b. Recursively call dfs() on the neighbour.
//  4. When no unvisited neighbours remain, backtrack
//     (the call returns – this IS the backtracking step).
//  5. Repeat until every cell has been visited.
//
//  The result is a *perfect* maze: exactly one path between
//  any two cells, guaranteed no unreachable areas.
// ============================================================

#include "Maze.h"
#include <algorithm>
#include <random>
#include <array>

Maze::Maze(int rows, int cols, unsigned seed)
    : m_rows(rows), m_cols(cols), m_seed(seed)
{
    // Initialise grid – all walls intact, nothing visited
    m_grid.assign(rows, std::vector<Cell>(cols));
    generate();
}

void Maze::generate()
{
    // Reset
    for (auto& row : m_grid)
        for (auto& c : row)
            c = Cell{};

    // Seed the RNG then kick off DFS from top-left
    srand(m_seed);
    dfs(0, 0);

    // Mark exit cell
    m_grid[exitRow()][exitCol()].isExit = true;
}

// ── recursive DFS carver ───────────────────────────────────
void Maze::dfs(int r, int c)
{
    m_grid[r][c].visited = true;

    // Directions: {dr, dc, wallInCurrent, wallInNeighbour}
    // 0=N,1=S,2=E,3=W  (indices match canMove convention)
    struct Dir { int dr, dc, wCur, wNbr; };
    std::array<Dir, 4> dirs = {{
        {-1,  0, 0, 1},   // North: this S-wall of neighbour = wallS
        { 1,  0, 1, 0},   // South
        { 0,  1, 2, 3},   // East
        { 0, -1, 3, 2}    // West
    }};

    // Shuffle for random maze topology
    for (int i = 3; i > 0; --i)
        std::swap(dirs[i], dirs[rand() % (i + 1)]);

    for (auto& d : dirs)
    {
        int nr = r + d.dr;
        int nc = c + d.dc;

        if (nr < 0 || nr >= m_rows || nc < 0 || nc >= m_cols) continue;
        if (m_grid[nr][nc].visited) continue;

        // ── Carve: remove the wall between (r,c) and (nr,nc) ──
        // Direction index → which wall member to clear
        auto clearWall = [](Cell& cell, int dir) {
            switch(dir) {
                case 0: cell.wallN = false; break;
                case 1: cell.wallS = false; break;
                case 2: cell.wallE = false; break;
                case 3: cell.wallW = false; break;
            }
        };
        clearWall(m_grid[r][c],   d.wCur);
        clearWall(m_grid[nr][nc], d.wNbr);

        dfs(nr, nc);   // recurse – backtrack happens on return
    }
}

// ── movement validity ──────────────────────────────────────
bool Maze::canMove(int r, int c, int dir) const
{
    const Cell& cell = m_grid[r][c];
    switch (dir) {
        case 0: return !cell.wallN && r > 0;
        case 1: return !cell.wallS && r < m_rows - 1;
        case 2: return !cell.wallE && c < m_cols  - 1;
        case 3: return !cell.wallW && c > 0;
    }
    return false;
}
