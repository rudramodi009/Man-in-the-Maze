#pragma once
// ============================================================
//  Maze.h  –  Data structure & DFS generation
// ============================================================
//  DATA STRUCTURE : 2-D vector<vector<Cell>> (grid / graph)
//  ALGORITHM       : Recursive DFS with backtracking
//
//  Each Cell stores whether it is a WALL or a PATH and which
//  of its four walls (N/E/S/W) have been carved open.
//
//  Time  complexity of DFS maze gen : O(R * C)  – every cell visited once
//  Space complexity                  : O(R * C)  – grid + O(R*C) call stack
// ============================================================

#include <vector>
#include <cstddef>

// ── cell type ──────────────────────────────────────────────
struct Cell
{
    bool visited   = false;   // used during generation
    bool wallN     = true;    // North wall present?
    bool wallS     = true;
    bool wallE     = true;
    bool wallW     = true;
    bool isExit    = false;
};

// ── Maze class ─────────────────────────────────────────────
class Maze
{
public:
    // Build a rows×cols maze; seed controls randomness
    Maze(int rows, int cols, unsigned seed = 42);

    // Generate using DFS + backtracking (called by ctor)
    void generate();

    // Accessors
    int  rows()  const { return m_rows; }
    int  cols()  const { return m_cols; }
    const Cell& cell(int r, int c) const { return m_grid[r][c]; }

    // Convenience: can the player step from (r,c) in direction dir?
    // dir: 0=N 1=S 2=E 3=W
    bool canMove(int r, int c, int dir) const;

    // Start and exit positions
    int startRow() const { return 0; }
    int startCol() const { return 0; }
    int exitRow()  const { return m_rows - 1; }
    int exitCol()  const { return m_cols  - 1; }

private:
    int  m_rows, m_cols;
    unsigned m_seed;
    std::vector<std::vector<Cell>> m_grid;

    // Recursive DFS carver
    void dfs(int r, int c);
};
