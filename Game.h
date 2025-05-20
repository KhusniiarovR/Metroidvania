#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "globals.h"
#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "graphics.h"
#include "assets.h"
#include "constants/game_elements.h"
#include "constants/physics.h"

class Game {
public:
    static void Init() {
        SetConfigFlags(FLAG_VSYNC_HINT);
        InitWindow(GetScreenWidth(), GetScreenHeight(), "Platformer");
        SetTargetFPS(60);
        //ToggleFullscreen();
        HideCursor();
        load_fonts();
        load_images();
        load_sounds();
        Level::get_instance().reset_data();
        Level::get_instance().load_level(1);
    }

    void Run() {
        while (!WindowShouldClose()) {
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

    static void update() {
        game_frame++;

        switch (game_state) {
            case MENU_STATE:
                if (IsKeyPressed(KEY_ENTER)) {
                    SetExitKey(0);
                    game_state = PLAY_STATE;
                    Level::get_instance().load_level(1);
                }

                if (IsKeyPressed(KEY_ESCAPE)) {
                    CloseWindow();
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
                Player::get_instance().set_on_ground(Level::get_instance().is_colliding({Player::get_instance().get_position().x, Player::get_instance().get_position().y + 0.1f}, game_elements::WALL)
                                 ||  Level::get_instance().is_colliding({Player::get_instance().get_position().x, Player::get_instance().get_position().y + 0.1f}, game_elements::PLATFORM));
                if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && Player::get_instance().get_is_on_ground()) {
                    Player::get_instance().set_y_velocity(-physics::JUMP_STRENGTH);
                }

                Player::get_instance().update();
                Enemy::get_instance().update();

                if (IsKeyPressed(KEY_ESCAPE)) {
                    game_state = PAUSED_STATE;
                }
                break;

            case PAUSED_STATE:
                if (IsKeyPressed(KEY_ESCAPE)) {
                    game_state = PLAY_STATE;
                }
                if (IsKeyPressed(KEY_ENTER)) {
                    game_state = MENU_STATE;
                }
                break;

            case DEATH_STATE:
                Player::get_instance().update_gravity();

                if (IsKeyPressed(KEY_ENTER)) {
                    if (Player::get_instance().get_lives() > 0) {
                        Level::get_instance().load_level(Level::get_instance().get_index());
                        game_state = PLAY_STATE;
                    }
                    else {
                        game_state = GAME_OVER_STATE;
                        PlaySound(game_over_sound);
                    }
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    CloseWindow();
                }
                break;

            case GAME_OVER_STATE:
                if (IsKeyPressed(KEY_ENTER)) {
                    Level::get_instance().reset_index();
                    Player::get_instance().reset_stats();
                    Level::get_instance().reset_data();
                    game_state = PLAY_STATE;
                    Level::get_instance().load_level(1);
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    game_state = MENU_STATE;
                }
                break;

            case VICTORY_STATE:
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                    Level::get_instance().reset_index();
                    Player::get_instance().reset_stats();
                    game_state = MENU_STATE;
                    SetExitKey(KEY_ESCAPE);
                }
                break;
        }
    }

    static void draw() {
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

    static void end_program() {
        unload_sounds();
        unload_images();
        unload_fonts();
        CloseAudioDevice();
        CloseWindow();
    }
};


#endif //GAME_H
