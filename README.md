# Man-in-the-Maze
Man in the Maze is a visually polished 2D maze escape game built in C++ using the SFML graphics library. The game generates a fully random maze using the Depth-First Search (DFS) Backtracking Algorithm, where the player must navigate through walls, avoid dead ends, and reach the glowing exit.
# 🌀 Man in the Maze

A visually polished **2D maze escape game** built using **C++** and **SFML**.  
Navigate through a procedurally generated maze, avoid dead ends, and reach the glowing green exit before losing your sanity in the labyrinth.

---

## 🎮 Gameplay

You control a glowing explorer trapped inside a randomly generated maze.

Your objective is simple:

> Reach the **GREEN EXIT** in the shortest time and minimum number of steps.

The maze is generated every time using the **Depth-First Search (DFS) Backtracking Algorithm**, making each run unique.

---

## ✨ Features

- 🧠 Procedural Maze Generation using DFS + Backtracking
- 🎨 Smooth SFML Graphics & Animations
- 🌌 Neon-inspired Visual Theme
- ⏱️ Real-time Timer & Step Counter
- 💥 Collision Detection System
- 🔁 Replay System (Press `R` after winning)
- 🎮 Keyboard Controls (WASD + Arrow Keys)
- ⚡ Lightweight and Fast

---

## 🛠️ Technologies Used

- **C++17**
- **SFML 2.5**
- **Object-Oriented Programming**
- **Recursive Algorithms**
- **CMake & Makefile**

---

## 📂 Project Structure

```bash
ManInTheMaze/
│
├── src/
│   ├── main.cpp
│   ├── Game.cpp
│   ├── Game.h
│   ├── Maze.cpp
│   ├── Maze.h
│   ├── Player.cpp
│   └── Player.h
│
├── CMakeLists.txt
├── Makefile
├── ManInTheMaze.exe
└── README.md
