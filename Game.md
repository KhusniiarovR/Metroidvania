# **Installation**
## Requirements:
### Operating System: Windows
### Compiler Toolchain: Visual Studio (MSVC)
### Library: raylib (installed via vcpkg)
### Build System: CMake

## The game is written in C++ using only the raylib library.

# **Code information:**
## *The project has three classes built according to the singleton pattern:*
### *Player - Responsible for the player logic*
### *Enemy - Responsible for the enemy logic*
### *Level - Responsible for the level logic*

## **Another Class:**
### *Game - Responsible for main game loop*

## **Additional .h files that works globally:**
### *assets.h, globals.h, graphics.h, utilities.h*
### *constants directory contains constants*

## **Data directory:**
### *fonts*
### *images (sprites)*
### *saves (level data encoded using a specific run-length encoding scheme)*
### *sounds*


# Gameplay:
### *Platformer game in which the levels are interconnected using a 4-exit system that tracks whether the player has gone beyond the boundaries of the current level*
### *The goal is to collect all the coins and complete all the levels fast for this there is a timer on top*
### *After losing or restarting, all collected coins are reset, allowing player to collect them again.*