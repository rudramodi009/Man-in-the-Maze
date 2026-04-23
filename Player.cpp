// ============================================================
//  Player.cpp  –  Player movement & collision detection
// ============================================================
//
//  COLLISION DETECTION
//  ───────────────────
//  When the player presses a key the game calls player.move().
//  move() delegates to maze.canMove(row, col, dir) which
//  checks the wall flag of the current cell in the requested
//  direction.  If the wall is intact → movement is blocked.
//  This is O(1) constant time per move because the walls are
//  stored directly in the Cell struct.
//
//  WIN CONDITION
//  ─────────────
//  After every successful move we test whether the player's
//  (row, col) equals the maze exit position.  O(1) check.
// ============================================================

#include "Player.h"

Player::Player(int startRow, int startCol)
    : m_row(startRow), m_col(startCol)
{}

bool Player::move(int dir, const Maze& maze)
{
    if (!maze.canMove(m_row, m_col, dir))
        return false;          // wall blocks movement – collision detected

    // Apply movement delta
    switch (dir) {
        case 0: --m_row; break;  // North
        case 1: ++m_row; break;  // South
        case 2: ++m_col; break;  // East
        case 3: --m_col; break;  // West
    }
    return true;
}

bool Player::hasWon(const Maze& maze) const
{
    return m_row == maze.exitRow() && m_col == maze.exitCol();
}
