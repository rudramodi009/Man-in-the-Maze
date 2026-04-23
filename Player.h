#pragma once
// ============================================================
//  Player.h  –  Grid-based player with collision detection
// ============================================================
//  The Player stores its position as (row, col) grid coords.
//  Movement is delegated to Maze::canMove() which checks the
//  wall flags carved during generation → O(1) per move.
// ============================================================

#include "Maze.h"

class Player
{
public:
    Player(int startRow, int startCol);

    // Attempt to move in direction dir (0=N,1=S,2=E,3=W)
    // Returns true if move was legal (no wall blocking)
    bool move(int dir, const Maze& maze);

    int row() const { return m_row; }
    int col() const { return m_col; }

    // True once player lands on exit cell
    bool hasWon(const Maze& maze) const;

private:
    int m_row, m_col;
};
