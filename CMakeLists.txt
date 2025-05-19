set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.27)
project(platformer)

set(CMAKE_CXX_STANDARD 17)


find_package(raylib CONFIG REQUIRED)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fsanitize=address -fsanitize=undefined")
endif()

add_executable(
        platformer platformer.cpp
        globals.h
        graphics.h
        level.h level.cpp
        player.h player.cpp
        enemy.h enemy.cpp
        assets.h
        utilities.h
        Game.h
        constants/game_elements.h
        constants/physics.h
)
target_link_libraries(platformer PRIVATE raylib)
