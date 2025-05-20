#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "globals.h"
#include "level.h"
#include "Player.h"
#include "enemy.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"
#include "constants/game_elements.h"
#include "constants/physics.h"

Enemy enemy;
Level level;

class game {
public:
    void Init() {
        SetConfigFlags(FLAG_VSYNC_HINT);
        InitWindow(GetScreenWidth(), GetScreenHeight(), "Platformer");
        SetTargetFPS(60);
        HideCursor();
        load_fonts();
        load_images();
        load_sounds();
        level.load_level(1);
        running = true;
    }

    void Run() {
        while (running && !WindowShouldClose()) {
            BeginDrawing();

            update();
            draw();

            EndDrawing();
        }

        end_program();
    }

private:
    size_t gameFrame = 0;
    int timer = 0;
    bool running = false;


void update() {
    game_frame++;

    switch (game_state) {
        case MENU_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                SetExitKey(0);
                game_state = PLAY_STATE;
                level.load_level(1);
            }
            break;

        case PLAY_STATE:
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                Player::get_instance().move_horizontally(physics::PLAYER_MOVEMENT_SPEED);
            }

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                Player::get_instance().move_horizontally(-physics::PLAYER_MOVEMENT_SPEED);
            }

            if (IsKeyDown(KEY_EQUAL)) {
                Player::get_instance().increment_lifes();
            }

            // Calculating collisions to decide whether the player is allowed to jump
            Player::get_instance().set_on_ground(level.is_colliding({Player::get_instance().get_position().x, Player::get_instance().get_position().y + 0.1f}, game_elements::WALL)
                             ||  level.is_colliding({Player::get_instance().get_position().x, Player::get_instance().get_position().y + 0.1f}, game_elements::PLATFORM));
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && Player::get_instance().get_is_on_ground()) {
                Player::get_instance().set_y_velocity(-physics::JUMP_STRENGTH);
            }

            Player::get_instance().update();
            enemy.update();

            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = PAUSED_STATE;
            }
            break;

        case PAUSED_STATE:
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = PLAY_STATE;
            }
            break;

        case DEATH_STATE:
            Player::get_instance().update_gravity();

            if (IsKeyPressed(KEY_ENTER)) {
                if (Player::get_instance().get_lives() > 0) {
                    level.load_level(level.get_index());
                    game_state = PLAY_STATE;
                }
                else {
                    game_state = GAME_OVER_STATE;
                    level.reset_data();
                    PlaySound(game_over_sound);
                }
            }
            break;

        case GAME_OVER_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                level.reset_index();
                Player::get_instance().reset_stats();
                level.reset_data();
                game_state = PLAY_STATE;
                level.load_level(1);
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = MENU_STATE;
            }
            break;

        case VICTORY_STATE:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                level.reset_index();
                Player::get_instance().reset_stats();
                game_state = MENU_STATE;
                SetExitKey(KEY_ESCAPE);
            }
            break;
    }
}

void draw() {
    switch(game_state) {
        case MENU_STATE:
            ClearBackground(BLACK);
            draw_menu();
            break;

        case PLAY_STATE:
            ClearBackground(BLACK);
            draw_parallax_background();
            draw_level();
            draw_game_overlay();
            break;

        case DEATH_STATE:
            ClearBackground(BLACK);
            draw_death_screen();
            break;

        case GAME_OVER_STATE:
            ClearBackground(BLACK);
            draw_game_over_menu();
            break;

        case PAUSED_STATE:
            ClearBackground(BLACK);
            draw_pause_menu();
            break;

        case VICTORY_STATE:
            draw_victory_menu();
            break;
    }
}
    void end_program() {
    unload_sounds();
    unload_images();
    unload_fonts();
    CloseAudioDevice();
    CloseWindow();
    }
};


#endif //GAME_H
