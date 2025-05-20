#pragma once

#include "raylib.h"

namespace graphics{
    inline const float SCREEN_SCALE_DIVISOR = 700.0f;
    // to scale things accordingly to look pleasant.
    inline Vector2 screen_size;
    inline float screen_scale; // Used to scale str/UI components size and displacements based on the screen_size size
    inline float cell_size;
    inline float horizontal_shift;

    // Parallax background scrolling
    inline Vector2 background_size;
    inline float background_y_offset;

    inline const float PARALLAX_PLAYER_SCROLLING_SPEED = 0.003f;
    inline const float PARALLAX_IDLE_SCROLLING_SPEED = 0.00005f;
    inline const float PARALLAX_LAYERED_SPEED_DIFFERENCE = 3.0f;

}
