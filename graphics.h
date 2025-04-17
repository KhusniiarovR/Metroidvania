#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "globals.h"
#include "Player.h"
#include "enemy.h"

extern Player player;
extern Enemy enemy;
extern Level level;

void draw_text(Text &text) {
    // Measure the text, center it to the required position, and draw it
    Vector2 dimensions = MeasureTextEx(*text.font, text.str.c_str(), text.size * screen_scale, text.spacing);

    Vector2 pos = {
        (screen_size.x * text.position.x) - (0.5f * dimensions.x),
        (screen_size.y * text.position.y) - (0.5f * dimensions.y)
    };

    DrawTextEx(*text.font, text.str.c_str(), pos, dimensions.y, text.spacing, text.color);
}

void derive_graphics_metrics_from_loaded_level() {
    // Level and UI setup
    screen_size.x  = static_cast<float>(GetRenderWidth());
    screen_size.y = static_cast<float>(GetRenderHeight());

    cell_size = screen_size.y / static_cast<float>(level.get_rows());
    screen_scale = std::min(screen_size.x, screen_size.y) / SCREEN_SCALE_DIVISOR;

    // Parallax background setup
    float larger_screen_side = std::max(screen_size.x, screen_size.y);

    if (screen_size.x > screen_size.y) {
        background_size = {larger_screen_side, larger_screen_side / 16 * 10};
    }
    else {
        background_size = {larger_screen_side / 10 * 16, larger_screen_side};
    }

    background_y_offset = (screen_size.y - background_size.y) * 0.5f;
}

void draw_parallax_background() {
    // First uses the player's position
    float initial_offset      = -(player.get_position().x * PARALLAX_PLAYER_SCROLLING_SPEED + game_frame * PARALLAX_IDLE_SCROLLING_SPEED);

    // Calculate offsets for different layers
    float background_offset   = initial_offset;
    float middleground_offset = background_offset * PARALLAX_LAYERED_SPEED_DIFFERENCE;
    float foreground_offset   = middleground_offset * PARALLAX_LAYERED_SPEED_DIFFERENCE;

    // Wrap offsets to create a loop effect
    background_offset   = fmod(background_offset, 1.0f);
    middleground_offset = fmod(middleground_offset, 1.0f);
    foreground_offset   = fmod(foreground_offset, 1.0f);

    // Scale to background size
    background_offset   *= background_size.x;
    middleground_offset *= background_size.x;
    foreground_offset   *= background_size.x;

    // Each layer is drawn twice, side by side, the first starting from its offset, and the other from its offset + background_size
    // This ensures a seamless scrolling effect, because when one copy moves out of sight, the second jumps into its place.
    draw_image(background,   {background_offset + background_size.x, background_y_offset},   background_size.x, background_size.y);
    draw_image(background,   {background_offset,                     background_y_offset},   background_size.x, background_size.y);

    draw_image(middleground, {middleground_offset + background_size.x, background_y_offset}, background_size.x, background_size.y);
    draw_image(middleground, {middleground_offset,                     background_y_offset}, background_size.x, background_size.y);

    draw_image(foreground,   {foreground_offset + background_size.x, background_y_offset},   background_size.x, background_size.y);
    draw_image(foreground,   {foreground_offset,                     background_y_offset},   background_size.x, background_size.y);
}

void draw_game_overlay() {
    const float ICON_SIZE = 48.0f * screen_scale;

    float slight_vertical_offset = 8.0f;
    slight_vertical_offset *= screen_scale;

    // Hearts
    for (int i = 0; i < player.get_lives(); i++) {
        const float SPACE_BETWEEN_HEARTS = 4.0f * screen_scale;
        draw_image(heart_image, {ICON_SIZE * i + SPACE_BETWEEN_HEARTS, slight_vertical_offset}, ICON_SIZE);
    }

    // Timer
    Vector2 timer_dimensions = MeasureTextEx(menu_font, std::to_string(timer / 60).c_str(), ICON_SIZE, 2.0f);
    Vector2 timer_position = {(GetRenderWidth() - timer_dimensions.x) * 0.5f, slight_vertical_offset};
    DrawTextEx(menu_font, std::to_string(timer / 60).c_str(), timer_position, ICON_SIZE, 2.0f, WHITE);

    // Score
    Vector2 score_dimensions = MeasureTextEx(menu_font, std::to_string(player.get_total_score()).c_str(), ICON_SIZE, 2.0f);
    Vector2 score_position = {GetRenderWidth() - score_dimensions.x - ICON_SIZE, slight_vertical_offset};
    DrawTextEx(menu_font, std::to_string(player.get_total_score()).c_str(), score_position, ICON_SIZE, 2.0f, WHITE);
    draw_sprite(coin_sprite, {GetRenderWidth() - ICON_SIZE, slight_vertical_offset}, ICON_SIZE);
}

void draw_level() {
    // Move the x-axis' center to the middle of the screen
    horizontal_shift = (screen_size.x - cell_size) / 2;

    for (size_t row = 0; row < level.get_rows(); ++row) {
        for (size_t column = 0; column < level.get_columns(); ++column) {

            Vector2 pos = {
                    // Move the level to the left as the player advances to the right,
                    // shifting to the left to allow the player to be centered later
                    (static_cast<float>(column) - player.get_position().x) * cell_size + horizontal_shift,
                    static_cast<float>(row) * cell_size
            };

            // Draw the level itself
            char cell = level.get_cell(row, column);
            switch (cell) {
                case WALL:
                    draw_image(wall_image, pos, cell_size);
                    break;
                case WALL_DARK:
                    draw_image(wall_dark_image, pos, cell_size);
                    break;
                case SPIKE:
                    draw_image(spike_image, pos, cell_size);
                    break;
                case COIN:
                    draw_sprite(coin_sprite, pos, cell_size);
                    break;
                case EXIT:
                    draw_image(exit_image, pos, cell_size);
                    break;
                default:
                    break;
            }
        }
    }

    draw_player();
    enemy.draw();
}

void draw_player() {
    horizontal_shift = (screen_size.x - cell_size) / 2;

    // Shift the camera to the center of the screen to allow to see what is in front of the player
    Vector2 pos = {
            horizontal_shift,
            player.get_position().y * cell_size
    };

    // Pick an appropriate sprite for the player
    if (game_state == GAME_STATE) {
        if (!player.get_is_on_ground()) {
            draw_image((player.get_is_looking_forward() ? player_jump_forward_image : player_jump_backwards_image), pos, cell_size);
        }
        else if (player.get_is_moving()) {
            draw_sprite((player.get_is_looking_forward() ? player_walk_forward_sprite : player_walk_backwards_sprite), pos, cell_size);
            player.set_moving(false);
        }
        else {
            draw_image((player.get_is_looking_forward() ? player_stand_forward_image : player_stand_backwards_image), pos, cell_size);
        }
    }
    else {
        draw_image(player_dead_image, pos, cell_size);
    }
}



// Menus
void draw_menu() {
    draw_text(game_title);
    draw_text(game_subtitle);
}

void draw_pause_menu() {
    draw_text(game_paused);
}

void draw_death_screen() {
    draw_parallax_background();
    draw_level();
    draw_game_overlay();
    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), {0, 0, 0, 100});
    draw_text(death_title);
    draw_text(death_subtitle);
}

void draw_game_over_menu() {
    draw_text(game_over_title);
    draw_text(game_over_subtitle);
}

void create_victory_menu_background() {
    for (auto &ball : victory_balls) {
        ball.x  = rand_up_to(screen_size.x);
        ball.y  = rand_up_to(screen_size.y);
        ball.dx = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
        ball.dx *= screen_scale;
        if (abs(ball.dx) < 0E-1) ball.dx = 1.0f;
        ball.dy = rand_from_to(-VICTORY_BALL_MAX_SPEED, VICTORY_BALL_MAX_SPEED);
        ball.dy *= screen_scale;
        if (abs(ball.dy) < 0E-1) ball.dy = 1.0f;
        ball.radius = rand_from_to(VICTORY_BALL_MIN_RADIUS, VICTORY_BALL_MAX_RADIUS);
        ball.radius *= screen_scale;
    }

    /* Clear both the front buffer and the back buffer to avoid ghosting of the game graphics. */
    ClearBackground(BLACK);
    EndDrawing();
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
    BeginDrawing();
}

void animate_victory_menu_background() {
    for (auto &ball : victory_balls) {
        ball.x += ball.dx;
        if (ball.x - ball.radius < 0 ||
            ball.x + ball.radius >= screen_size.x) {
            ball.dx = -ball.dx;
        }
        ball.y += ball.dy;
        if (ball.y - ball.radius < 0 ||
            ball.y + ball.radius >= screen_size.y) {
            ball.dy = -ball.dy;
        }
    }
}

void draw_victory_menu_background() {
    for (auto &ball : victory_balls) {
        DrawCircleV({ ball.x, ball.y }, ball.radius, VICTORY_BALL_COLOR);
    }
}

void draw_victory_menu() {
    DrawRectangle(
        0, 0,
        static_cast<int>(screen_size.x), static_cast<int>(screen_size.y),
        { 0, 0, 0, VICTORY_BALL_TRAIL_TRANSPARENCY }
    );

    animate_victory_menu_background();
    draw_victory_menu_background();

    draw_text(victory_title);
    draw_text(victory_subtitle);
}

#endif //GRAPHICS_H
