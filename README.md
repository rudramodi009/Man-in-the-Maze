# Man in the Maze 🧩
A college-level DSA project: maze navigation game built in **C++ + SFML**.

---

## Architecture Overview

```
main.cpp
   └── Game          (game loop, SFML rendering, input)
         ├── Maze    (2-D grid + DFS generation)
         └── Player  (grid position + movement/collision)
```

---

## Data Structures Used

| Structure | Where | Why |
|-----------|-------|-----|
| `vector<vector<Cell>>` | `Maze` | 2-D grid; O(1) random access by (row,col) |
| `Cell` struct | `Maze::m_grid` | Stores 4 wall flags; avoids separate adjacency list |
| Implicit call stack | DFS recursion | Acts as the DFS frontier / backtrack stack |

---

## Algorithm: DFS with Backtracking

### Step-by-step

1. **Start** at cell (0, 0). Mark it visited.
2. **Shuffle** the four cardinal directions randomly.
3. **For each direction** – check if neighbour exists AND is unvisited.
4. **Carve** the shared wall between current cell and chosen neighbour.
5. **Recurse** into the neighbour (depth-first).
6. When no unvisited neighbours remain, **return** (= backtrack).
7. Repeat until all cells are visited → perfect maze.

### Complexity

| Metric | Value | Reason |
|--------|-------|--------|
| Time  (generation) | O(R × C) | Each cell visited exactly once |
| Space (grid)       | O(R × C) | One Cell per grid position |
| Space (stack)      | O(R × C) | Worst-case call-stack depth |
| Time  (move)       | O(1)     | Direct wall-flag lookup |
| Time  (win check)  | O(1)     | Integer comparison |

---

## Player Movement & Collision Detection

```
Player presses key
       │
       ▼
Player::move(dir, maze)
       │
       ▼
maze.canMove(row, col, dir)   ← checks wall flag in O(1)
       │
  ┌────┴────┐
  │ wall?   │
  Yes       No
  │         │
block     update (row,col)
           │
           ▼
       hasWon() check  ← O(1) integer compare
```

---

## OOP Design

| Class | Responsibility | OOP Principle |
|-------|---------------|---------------|
| `Cell` | Data-only struct for one grid square | Single Responsibility |
| `Maze` | Grid ownership + DFS generation | Encapsulation |
| `Player` | Position + movement rules | Encapsulation |
| `Game` | SFML loop, render, orchestration | Separation of Concerns |

---

## Why SFML over SDL?

| | SFML | SDL |
|-|------|-----|
| Language | Pure C++ | C with C++ wrappers |
| 2-D shapes | Built-in `RectangleShape`, `CircleShape` | Manual texture/renderer |
| Font/text | Built-in `sf::Text` | Need SDL_ttf extension |
| Learning curve | Lower for C++ beginners | Higher |
| Grid games | Ideal | Ideal but more boilerplate |

---

## Build & Run Instructions

### Prerequisites
Install SFML 2.5+:
- **Ubuntu/Debian**: `sudo apt install libsfml-dev`
- **Fedora**: `sudo dnf install SFML-devel`
- **macOS**: `brew install sfml`
- **Windows**: Download from https://sfml-dev.org and configure CMake prefix

---

### Option A – Makefile (Linux/macOS)
```bash
cd ManInTheMaze
make
./ManInTheMaze
# Optional args: rows cols cellSize seed
./ManInTheMaze 20 20 36 999
```

### Option B – CMake (cross-platform)
```bash
cd ManInTheMaze
mkdir build && cd build
cmake ..
cmake --build . --config Release
./ManInTheMaze          # Linux/macOS
ManInTheMaze.exe        # Windows
```

---

## Controls

| Key | Action |
|-----|--------|
| `W` or `↑` | Move North |
| `S` or `↓` | Move South |
| `D` or `→` | Move East  |
| `A` or `←` | Move West  |
| `R` (after win) | New maze |

---

## Colour Legend

| Colour | Meaning |
|--------|---------|
| Dark navy | Walls |
| Soft lavender | Passable path |
| Green | Exit cell |
| Gold circle | Player (you) |

---

## Extension Ideas (for higher marks)
- **BFS solver** to show the shortest solution path
- **Prim's Algorithm** as alternative maze generator
- **Timer** / move counter HUD
- **Multiple difficulty levels** (bigger grid, thinner passages)
- **Animated** player sprite with sprite-sheet
