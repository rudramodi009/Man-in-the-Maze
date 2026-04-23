#include "Game.h"
#include <sstream>
#include <ctime>
#include <cmath>
#include <iomanip>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const sf::Color COL_BG        (12,  12,  20);     // deep space black
static const sf::Color COL_WALL      (55,  45,  90);     // muted purple
static const sf::Color COL_WALL_HI   (75,  60, 120);     // wall highlight
static const sf::Color COL_PATH_A    (42,  40,  62);     // floor gradient top
static const sf::Color COL_PATH_B    (34,  32,  52);     // floor gradient bottom
static const sf::Color COL_EXIT      (60, 220, 130);     // vibrant green
static const sf::Color COL_PLAYER    (255, 200,  60);     // warm gold
static const sf::Color COL_PLAYER_OUT(220, 160,  30);     // gold outline
static const sf::Color COL_GLOW      (255, 220, 100, 35); // player glow
static const sf::Color COL_HUD_BG    (18,  16,  30);     // HUD background
static const sf::Color COL_HUD_LINE  (70,  60, 110);     // HUD separator
static const sf::Color COL_TEXT_PRI  (210, 210, 240);     // primary text
static const sf::Color COL_TEXT_SEC  (130, 125, 170);     // secondary text
static const sf::Color COL_TEXT_ACC  (100, 220, 150);     // accent text (green)
static const sf::Color COL_GRID_LINE (50,  45,  75, 100); // subtle grid lines

static sf::Color lerpColor(const sf::Color& a, const sf::Color& b, float t) {
    return sf::Color(
        (sf::Uint8)(a.r + (b.r - a.r) * t),
        (sf::Uint8)(a.g + (b.g - a.g) * t),
        (sf::Uint8)(a.b + (b.b - a.b) * t),
        (sf::Uint8)(a.a + (b.a - a.a) * t)
    );
}

Game::Game(int rows, int cols, int cellSize, unsigned seed)
    : m_maze(rows, cols, seed)
    , m_player(m_maze.startRow(), m_maze.startCol())
    , m_rows(rows)
    , m_cols(cols)
    , m_cellSize(cellSize)
    , m_won(false)
    , m_moveCount(0)
{
    const int HUD_H = 70;
    int winW = cols * cellSize;
    int winH = rows * cellSize + HUD_H;

    m_window.create(
        sf::VideoMode(winW, winH),
        "Man in the Maze",
        sf::Style::Titlebar | sf::Style::Close
    );
    m_window.setFramerateLimit(60);

    bool fontLoaded = false;
    std::vector<std::string> fontPaths = {
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        "/System/Library/Fonts/Helvetica.ttc"
    };
    for (auto& p : fontPaths) {
        if (m_font.loadFromFile(p)) { fontLoaded = true; break; }
    }
    (void)fontLoaded;
}


void Game::run()
{
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();

            // Restart on R key after winning
            if (m_won && event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::R)
            {
                m_maze      = Maze(m_rows, m_cols, (unsigned)time(nullptr));
                m_player    = Player(m_maze.startRow(), m_maze.startCol());
                m_won       = false;
                m_moveCount = 0;
                m_gameClock.restart();
                m_animClock.restart();
            }
        }

        if (!m_won) handleInput();
        render();
    }
}

// ── input + movement ──────────────────────────────────────
void Game::handleInput()
{
    if (m_moveClock.getElapsedTime().asMilliseconds() < 120) return;

    int dir = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    dir = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  dir = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dir = 2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  dir = 3;

    if (dir != -1)
    {
        if (m_player.move(dir, m_maze)) {
            m_moveCount++;
        }
        m_moveClock.restart();
        if (m_player.hasWon(m_maze))
            m_won = true;
    }
}

// ── rendering ─────────────────────────────────────────────
void Game::render()
{
    m_window.clear(COL_BG);
    drawMaze();
    drawPlayer();
    drawHUD();
    if (m_won)
        drawOverlay("YOU ESCAPED!");
    m_window.display();
}

// ── draw maze cells and walls ─────────────────────────────
void Game::drawMaze()
{
    const float CS  = static_cast<float>(m_cellSize);
    const float WT  = std::max(2.f, CS * 0.10f);   // wall thickness
    const float t   = m_animClock.getElapsedTime().asSeconds();

    sf::RectangleShape cellRect({CS, CS});
    sf::RectangleShape wallRect;

    for (int r = 0; r < m_rows; ++r)
    {
        for (int c = 0; c < m_cols; ++c)
        {
            float x = c * CS;
            float y = r * CS;
            const Cell& cell = m_maze.cell(r, c);

            // ── floor tile with subtle gradient based on position ──
            // Create a slight color variation per cell for a "tiled" look
            float gradT = (float)(r + c) / (float)(m_rows + m_cols);
            sf::Color floorCol = lerpColor(COL_PATH_A, COL_PATH_B, gradT);

            // Slight checkerboard brightness variation
            if ((r + c) % 2 == 0) {
                floorCol.r = std::min(255, floorCol.r + 6);
                floorCol.g = std::min(255, floorCol.g + 6);
                floorCol.b = std::min(255, floorCol.b + 8);
            }

            cellRect.setPosition(x, y);
            cellRect.setFillColor(floorCol);
            m_window.draw(cellRect);

            // ── exit cell: pulsing green glow ──
            if (cell.isExit) {
                float pulse = 0.55f + 0.45f * std::sin(t * 3.0f);
                sf::Uint8 ga = (sf::Uint8)(pulse * 80);
                // Outer glow
                float glowR = CS * 0.7f;
                sf::CircleShape exitGlow(glowR);
                exitGlow.setFillColor(sf::Color(60, 220, 130, ga));
                exitGlow.setPosition(x + CS / 2.f - glowR, y + CS / 2.f - glowR);
                m_window.draw(exitGlow);
                // Inner fill
                cellRect.setFillColor(sf::Color(
                    (sf::Uint8)(40 + pulse * 30),
                    (sf::Uint8)(160 + pulse * 60),
                    (sf::Uint8)(80 + pulse * 40)
                ));
                cellRect.setPosition(x, y);
                m_window.draw(cellRect);
            }

            // ── subtle grid lines ──
            sf::RectangleShape gridH({CS, 1.f});
            gridH.setPosition(x, y);
            gridH.setFillColor(COL_GRID_LINE);
            m_window.draw(gridH);

            sf::RectangleShape gridV({1.f, CS});
            gridV.setPosition(x, y);
            gridV.setFillColor(COL_GRID_LINE);
            m_window.draw(gridV);

            // ── draw walls ──
            // Main wall color
            wallRect.setFillColor(COL_WALL);

            // North wall
            if (cell.wallN) {
                wallRect.setSize({CS + WT, WT});
                wallRect.setPosition(x - WT / 2.f, y - WT / 2.f);
                m_window.draw(wallRect);
                // highlight stripe on top edge
                sf::RectangleShape hi({CS + WT, 1.f});
                hi.setPosition(x - WT / 2.f, y - WT / 2.f);
                hi.setFillColor(COL_WALL_HI);
                m_window.draw(hi);
            }
            // South wall
            if (cell.wallS) {
                wallRect.setSize({CS + WT, WT});
                wallRect.setPosition(x - WT / 2.f, y + CS - WT / 2.f);
                m_window.draw(wallRect);
                sf::RectangleShape hi({CS + WT, 1.f});
                hi.setPosition(x - WT / 2.f, y + CS - WT / 2.f);
                hi.setFillColor(COL_WALL_HI);
                m_window.draw(hi);
            }
            // East wall
            if (cell.wallE) {
                wallRect.setSize({WT, CS + WT});
                wallRect.setPosition(x + CS - WT / 2.f, y - WT / 2.f);
                m_window.draw(wallRect);
                sf::RectangleShape hi({1.f, CS + WT});
                hi.setPosition(x + CS - WT / 2.f, y - WT / 2.f);
                hi.setFillColor(COL_WALL_HI);
                m_window.draw(hi);
            }
            // West wall
            if (cell.wallW) {
                wallRect.setSize({WT, CS + WT});
                wallRect.setPosition(x - WT / 2.f, y - WT / 2.f);
                m_window.draw(wallRect);
                sf::RectangleShape hi({1.f, CS + WT});
                hi.setPosition(x + WT * 0.3f, y - WT / 2.f);
                hi.setFillColor(COL_WALL_HI);
                m_window.draw(hi);
            }
        }
    }
}

// ── draw player with glow ring ────────────────────────────
void Game::drawPlayer()
{
    const float CS = static_cast<float>(m_cellSize);
    const float R  = CS * 0.32f;
    const float t  = m_animClock.getElapsedTime().asSeconds();

    float cx = m_player.col() * CS + CS / 2.f;
    float cy = m_player.row() * CS + CS / 2.f;

    // ── outer glow ring (pulsing) ──
    float glowPulse = 0.6f + 0.4f * std::sin(t * 2.5f);
    float glowR = R * (1.6f + 0.15f * std::sin(t * 2.5f));
    sf::CircleShape glow(glowR);
    glow.setFillColor(sf::Color(255, 220, 100, (sf::Uint8)(glowPulse * 40)));
    glow.setPosition(cx - glowR, cy - glowR);
    m_window.draw(glow);

    // ── second glow layer (smaller, brighter) ──
    float glow2R = R * 1.25f;
    sf::CircleShape glow2(glow2R);
    glow2.setFillColor(sf::Color(255, 210, 80, (sf::Uint8)(glowPulse * 55)));
    glow2.setPosition(cx - glow2R, cy - glow2R);
    m_window.draw(glow2);

    // ── main body circle ──
    sf::CircleShape body(R);
    body.setFillColor(COL_PLAYER);
    body.setOutlineColor(COL_PLAYER_OUT);
    body.setOutlineThickness(2.f);
    body.setPosition(cx - R, cy - R);
    m_window.draw(body);

    // ── inner shine (specular highlight) ──
    float shineR = R * 0.35f;
    sf::CircleShape shine(shineR);
    shine.setFillColor(sf::Color(255, 245, 200, 120));
    shine.setPosition(cx - R * 0.35f, cy - R * 0.45f);
    m_window.draw(shine);

    // ── eyes (blink animation) ──
    float blinkCycle = std::fmod(t, 3.0f);
    bool eyesOpen = (blinkCycle < 2.7f || blinkCycle > 2.85f);
    if (eyesOpen) {
        float eyeR = R * 0.12f;
        sf::CircleShape eyeL(eyeR);
        eyeL.setFillColor(sf::Color(50, 30, 10));
        eyeL.setPosition(cx - R * 0.25f - eyeR, cy - R * 0.05f - eyeR);
        m_window.draw(eyeL);

        sf::CircleShape eyeR2(eyeR);
        eyeR2.setFillColor(sf::Color(50, 30, 10));
        eyeR2.setPosition(cx + R * 0.15f - eyeR, cy - R * 0.05f - eyeR);
        m_window.draw(eyeR2);
    } else {
        // Closed eyes — thin lines (small rectangles)
        sf::RectangleShape lineL({R * 0.22f, 1.f});
        lineL.setFillColor(sf::Color(50, 30, 10));
        lineL.setPosition(cx - R * 0.35f, cy - R * 0.05f);
        m_window.draw(lineL);

        sf::RectangleShape lineR({R * 0.22f, 1.f});
        lineR.setFillColor(sf::Color(50, 30, 10));
        lineR.setPosition(cx + R * 0.05f, cy - R * 0.05f);
        m_window.draw(lineR);
    }

    // ── small mouth ──
    float mouthR = R * 0.08f;
    sf::CircleShape mouth(mouthR);
    mouth.setFillColor(sf::Color(60, 35, 10));
    mouth.setPosition(cx - mouthR, cy + R * 0.25f - mouthR);
    m_window.draw(mouth);
}

// ── HUD (heads-up display) bar ────────────────────────────
void Game::drawHUD()
{
    const float CS   = static_cast<float>(m_cellSize);
    const float hudY = m_rows * CS;
    const float hudW = static_cast<float>(m_window.getSize().x);
    const float hudH = 70.f;

    // ── background ──
    sf::RectangleShape bg({hudW, hudH});
    bg.setPosition(0.f, hudY);
    bg.setFillColor(COL_HUD_BG);
    m_window.draw(bg);

    // ── top separator line ──
    sf::RectangleShape sep({hudW, 2.f});
    sep.setPosition(0.f, hudY);
    sep.setFillColor(COL_HUD_LINE);
    m_window.draw(sep);

    // ── gradient accent bar ──
    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape accent({hudW, 1.f});
        accent.setPosition(0.f, hudY + 2.f + i);
        accent.setFillColor(sf::Color(100, 80, 160, (sf::Uint8)(80 - i * 25)));
        m_window.draw(accent);
    }

    // ── LEFT: Controls ──
    sf::Text controls;
    controls.setFont(m_font);
    controls.setCharacterSize(13);
    controls.setFillColor(COL_TEXT_SEC);
    controls.setString("W/A/S/D  or  Arrow Keys");
    controls.setPosition(14.f, hudY + 12.f);
    m_window.draw(controls);

    sf::Text goal;
    goal.setFont(m_font);
    goal.setCharacterSize(12);
    goal.setFillColor(COL_TEXT_ACC);
    goal.setString("Reach the GREEN exit");
    goal.setPosition(14.f, hudY + 34.f);
    m_window.draw(goal);

    // ── vertical separator ──
    sf::RectangleShape vsep1({1.f, hudH - 20.f});
    vsep1.setPosition(hudW * 0.38f, hudY + 10.f);
    vsep1.setFillColor(COL_HUD_LINE);
    m_window.draw(vsep1);

    // ── CENTER: Steps ──
    sf::Text stepsLabel;
    stepsLabel.setFont(m_font);
    stepsLabel.setCharacterSize(11);
    stepsLabel.setFillColor(COL_TEXT_SEC);
    stepsLabel.setString("STEPS");
    stepsLabel.setPosition(hudW * 0.46f, hudY + 8.f);
    m_window.draw(stepsLabel);

    sf::Text stepsVal;
    stepsVal.setFont(m_font);
    stepsVal.setCharacterSize(22);
    stepsVal.setFillColor(COL_TEXT_PRI);
    stepsVal.setString(std::to_string(m_moveCount));
    stepsVal.setPosition(hudW * 0.46f, hudY + 26.f);
    m_window.draw(stepsVal);

    // ── vertical separator ──
    sf::RectangleShape vsep2({1.f, hudH - 20.f});
    vsep2.setPosition(hudW * 0.62f, hudY + 10.f);
    vsep2.setFillColor(COL_HUD_LINE);
    m_window.draw(vsep2);

    // ── RIGHT: Timer ──
    int elapsed = (int)m_gameClock.getElapsedTime().asSeconds();
    int mins = elapsed / 60;
    int secs = elapsed % 60;
    std::ostringstream tss;
    tss << std::setw(2) << std::setfill('0') << mins << ":"
        << std::setw(2) << std::setfill('0') << secs;

    sf::Text timeLabel;
    timeLabel.setFont(m_font);
    timeLabel.setCharacterSize(11);
    timeLabel.setFillColor(COL_TEXT_SEC);
    timeLabel.setString("TIME");
    timeLabel.setPosition(hudW * 0.72f, hudY + 8.f);
    m_window.draw(timeLabel);

    sf::Text timeVal;
    timeVal.setFont(m_font);
    timeVal.setCharacterSize(22);
    timeVal.setFillColor(COL_TEXT_PRI);
    timeVal.setString(tss.str());
    timeVal.setPosition(hudW * 0.72f, hudY + 26.f);
    m_window.draw(timeVal);

    // ── Player position (small, bottom right) ──
    std::ostringstream pss;
    pss << "(" << m_player.row() << ", " << m_player.col() << ")";
    sf::Text posText;
    posText.setFont(m_font);
    posText.setCharacterSize(11);
    posText.setFillColor(COL_TEXT_SEC);
    posText.setString(pss.str());
    posText.setPosition(hudW - 80.f, hudY + 50.f);
    m_window.draw(posText);
}

// ── win overlay ───────────────────────────────────────────
void Game::drawOverlay(const std::string& msg)
{
    const float t = m_animClock.getElapsedTime().asSeconds();
    float winW = (float)m_window.getSize().x;
    float winH = (float)m_window.getSize().y;

    // ── dark veil ──
    sf::RectangleShape veil({winW, winH});
    veil.setFillColor(sf::Color(5, 5, 15, 180));
    m_window.draw(veil);

    // ── pulsing glow behind text ──
    float glowPulse = 0.6f + 0.4f * std::sin(t * 2.0f);
    float gr = 120.f;
    sf::CircleShape bgGlow(gr);
    bgGlow.setFillColor(sf::Color(60, 220, 130, (sf::Uint8)(glowPulse * 40)));
    bgGlow.setPosition(winW / 2.f - gr, winH / 2.f - gr - 30.f);
    m_window.draw(bgGlow);

    // ── main title ──
    sf::Text title;
    title.setFont(m_font);
    title.setCharacterSize(36);
    title.setFillColor(sf::Color(
        (sf::Uint8)(80 + glowPulse * 40),
        (sf::Uint8)(200 + glowPulse * 55),
        (sf::Uint8)(120 + glowPulse * 30)
    ));
    title.setStyle(sf::Text::Bold);
    title.setString(msg);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition((winW - tb.width) / 2.f, winH / 2.f - 60.f);
    m_window.draw(title);

    // ── stats line ──
    int elapsed = (int)m_gameClock.getElapsedTime().asSeconds();
    int mins = elapsed / 60;
    int secs = elapsed % 60;
    std::ostringstream ss;
    ss << m_moveCount << " steps  |  "
       << std::setw(2) << std::setfill('0') << mins << ":"
       << std::setw(2) << std::setfill('0') << secs;

    sf::Text stats;
    stats.setFont(m_font);
    stats.setCharacterSize(18);
    stats.setFillColor(COL_TEXT_PRI);
    stats.setString(ss.str());
    sf::FloatRect sb = stats.getLocalBounds();
    stats.setPosition((winW - sb.width) / 2.f, winH / 2.f + 0.f);
    m_window.draw(stats);

    // ── restart prompt ──
    sf::Text restart;
    restart.setFont(m_font);
    restart.setCharacterSize(14);
    restart.setFillColor(COL_TEXT_SEC);
    restart.setString("Press R to play again");
    sf::FloatRect rb = restart.getLocalBounds();
    restart.setPosition((winW - rb.width) / 2.f, winH / 2.f + 40.f);
    m_window.draw(restart);
}
