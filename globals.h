#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"

#include <vector>
#include <string>
#include <cstddef>
#include <cmath>

/* Timer-mechanic related */


/* Fonts */

inline Font menu_font;

/* Display Text Parameters */

struct Text {
    std::string str;
    Vector2 position = {0.50f, 0.50f};
    float size = 32.0f;
    Color color = WHITE;
    float spacing = 4.0f;
    Font* font = &menu_font;
};

/* Images and Sprites */

struct Sprite {
    size_t frame_count    = 0;
    size_t frames_to_skip = 3;
    size_t frames_skipped = 0;
    size_t frame_index    = 0;
    bool loop = true;
    size_t prev_game_frame = 0;
    Texture2D *frames = nullptr;
};

// Level Elements
inline Texture2D wall_image;
inline Texture2D wall_dark_image;
inline Texture2D spike_image;
inline Texture2D exit_image;
inline Texture2D platform_image;
inline Texture2D spring_image;
inline Texture2D menu_image;
inline Sprite coin_sprite;

// UI Elements
inline Texture2D heart_image;

// Player
inline Texture2D player_stand_forward_image;
inline Texture2D player_stand_backwards_image;
inline Texture2D player_jump_forward_image;
inline Texture2D player_jump_backwards_image;
inline Texture2D player_dead_image;
inline Sprite player_walk_forward_sprite;
inline Sprite player_walk_backwards_sprite;

// Enemy
inline Sprite enemy_walk;

// Background Elements
inline Texture2D background;
inline Texture2D middleground;
inline Texture2D foreground;

/* Sounds */

inline Sound coin_sound;
inline Sound exit_sound;
inline Sound player_death_sound;
inline Sound kill_enemy_sound;
inline Sound game_over_sound;
inline Sound spring_sound;

/* Victory Menu Background */

struct victory_ball {
    float x, y;
    float dx, dy;
    float radius;
};

inline const size_t VICTORY_BALL_COUNT     = 2000;
inline const float VICTORY_BALL_MAX_SPEED  = 2.0f;
inline const float VICTORY_BALL_MIN_RADIUS = 2.0f;
inline const float VICTORY_BALL_MAX_RADIUS = 3.0f;
inline const Color VICTORY_BALL_COLOR      = { 180, 180, 180, 255 };
inline const unsigned char VICTORY_BALL_TRAIL_TRANSPARENCY = 10;
inline victory_ball victory_balls[VICTORY_BALL_COUNT];

/* Frame Counter */

inline size_t game_frame = 0;

/* Game States */

enum game_state {
    MENU_STATE,
    PLAY_STATE,
    PAUSED_STATE,
    DEATH_STATE,
    GAME_OVER_STATE,
    VICTORY_STATE
};
inline game_state game_state = MENU_STATE;

/* Forward Declarations */

// GRAPHICS_H

void draw_text(Text &text);
void derive_graphics_metrics_from_loaded_level();
void draw_game_overlay();
void draw_level();
void draw_player();
void draw_menu();

void draw_pause_menu();
void draw_death_screen();
void draw_game_over_menu();

void create_victory_menu_background();
void animate_victory_menu_background();
void draw_victory_menu_background();
void draw_victory_menu();

// ASSETS_H

void load_fonts();
void unload_fonts();

void load_images();
void unload_images();

void draw_image(Texture2D image, Vector2 pos, float width, float height);
void draw_image(Texture2D image, Vector2 pos, float size);

Sprite load_sprite(
        const std::string &file_name_prefix,
        const std::string &file_name_suffix,
        size_t frame_count = 1,
        bool loop = true,
        size_t frames_to_skip = 3
);
void unload_sprite(Sprite &sprite);
void draw_sprite(Sprite &sprite, Vector2 pos, float width, float height);
void draw_sprite(Sprite &sprite, Vector2 pos, float size);

void load_sounds();
void unload_sounds();

#endif // GLOBALS_H