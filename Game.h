#pragma once
// ============================================================
//  Game.h  –  Top-level game loop (SFML)
// ============================================================
//  WHY SFML?
//  ─────────
//  • Lightweight, header/lib only install (no SDL_image etc.)
//  • Clean C++ API with no C casts
//  • Built-in sf::RenderWindow, sf::RectangleShape, sf::Text
//  • Ideal for grid-based 2-D games at college level
//  • Cross-platform: Windows / Linux / macOS
// ============================================================

#include <SFML/Graphics.hpp>
#include "Maze.h"
#include "Player.h"
#include <string>

class Game
{
public:
    // CELL_SIZE in pixels; rows/cols control maze complexity
    Game(int rows = 15, int cols = 15,
         int cellSize = 40, unsigned seed = 12345);

    // Enter the SFML event + render loop
    void run();

private:
    // ── sub-systems ───────────────────────────────────────
    Maze   m_maze;
    Player m_player;

    // ── SFML objects ──────────────────────────────────────
    sf::RenderWindow m_window;
    sf::Font         m_font;

    // ── config ────────────────────────────────────────────
    int m_cellSize;
    int m_rows, m_cols;
    bool m_won;
    sf::Clock m_moveClock;   // throttle key-repeat
    sf::Clock m_animClock;   // global animation timer
    sf::Clock m_gameClock;   // elapsed game time for HUD
    int       m_moveCount;   // step counter

    // ── helpers ───────────────────────────────────────────
    void handleInput();
    void render();
    void drawMaze();
    void drawPlayer();
    void drawHUD();
    void drawOverlay(const std::string& msg);
};
