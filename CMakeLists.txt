cmake_minimum_required(VERSION 3.16)
project(ManInTheMaze CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimisation flags
if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

# ── Sources ───────────────────────────────────────────────
set(SOURCES
    src/main.cpp
    src/Maze.cpp
    src/Player.cpp
    src/Game.cpp
)

add_executable(ManInTheMaze ${SOURCES})

# ── SFML ──────────────────────────────────────────────────
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(ManInTheMaze sfml-graphics sfml-window sfml-system)

# On Windows copy SFML DLLs next to the exe automatically
if(WIN32)
    add_custom_command(TARGET ManInTheMaze POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE_DIR:ManInTheMaze>)
endif()
