#include "player.h"
#include "enemy.h"
#include "globals.h"
#include "level.h"
#include <iostream>
#include "constants/game_elements.h"
#include "constants/physics.h"

extern Enemy enemy;
extern Level level;

Player::Player() : position({0.0f,0.0f}), y_velocity(0),
                   is_on_ground(true), is_moving(false),
                   is_looking_forward(true), init_player_pos_x(1), init_player_pos_y(1),
                   level_score(0), current_bound('D') {}

void Player::reset_stats() {
    lives = physics::START_LIFES;
    level_score = 0;
    current_bound = 'D';
}

void Player::spawn(float pos_x, float pos_y) {
    y_velocity = 0;

    position.x = pos_x;
    position.y = pos_y;
}

void Player::kill() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    lives--;
}

void Player::move_horizontally(float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    float next_x = position.x + delta;
    if (!level.is_colliding({next_x, position.y}, game_elements::WALL)) {
        position.x = next_x;
    }
    else {
        position.x = roundf(position.x);
        return;
    }

    // For drawing player animations
    is_looking_forward = delta > 0;
    if (delta != 0) is_moving = true;
}


void Player::update() {
    update_gravity();

    out_of_bounds();
    // Interacting with other level elements
    if (level.is_colliding(position, game_elements::COIN)) {
        level.get_collider(position, game_elements::COIN) = game_elements::AIR; // Removes the coin
        increment_score();
    }

    timer++;

    // Kill the player if they touch a spike or fall below the level
    if (level.is_colliding(position, game_elements::SPIKE)) {
        kill();
    }

    // Upon colliding with an enemy...
    if (enemy.is_colliding_enemy(position)) {
        // ...check if their velocity is downwards...
        if (y_velocity > 0) {
            // ...if yes, award the player and kill the enemy
            enemy.remove_colliding(position);
            PlaySound(kill_enemy_sound);
            y_velocity = -physics::BOUNCE_OFF_ENEMY;
        }
        else {
            // ...if not, kill the player
            kill();
        }
    }
}

void Player::update_gravity() {
    if (level.is_colliding({position.x, position.y - 0.1f}, game_elements::WALL) && y_velocity < 0) {
        y_velocity = physics::CEILING_BOUNCE_OFF;
    }

    if (level.is_colliding({position.x, position.y + 0.1f}, game_elements::SPRING)) {
        y_velocity = -physics::CEILING_BOUNCE_OFF * 8;
        PlaySound(spring_sound);
    }
    // Add gravity to player's y-position
    position.y += y_velocity;
    y_velocity += physics::GRAVITY_FORCE;

    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
    is_on_ground = level.is_colliding({position.x, position.y + 0.1f}, game_elements::WALL)
    || (
        level.is_colliding({position.x, position.y + 0.0001f}, game_elements::PLATFORM) &&
        y_velocity >= 0 &&
        !(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    );

    if (is_on_ground) {
        y_velocity = 0;
        position.y = roundf(position.y);
    }
}

void Player::out_of_bounds() {
    if (position.x < 0) {
        current_bound = 'L';
        level.load_level(level.get_bounds(1));
    }
    else if (position.x > level.get_columns()) {
        current_bound = 'R';
        level.load_level(level.get_bounds(2));
    }
    else if (position.y < 0) {
        current_bound = 'U';
        level.load_level(level.get_bounds(3));
    }
    else if (position.y > level.get_rows()) {
        current_bound = 'D';
        level.load_level(level.get_bounds(4));
    }
}

Vector2 Player::get_spawn_pos() const {
    for (int i = 0; i < level.get_rows(); i++) {
        for (int j = 0; j < level.get_columns(); j++) {
            if (level.get_cell(i, j) == current_bound) return {static_cast<float>(j), static_cast<float>(i)};
        }
    }

    for (int i = 0; i < level.get_rows(); i++) { // if forgot to add spawn priority left
        for (int j = 0; j < level.get_columns(); j++) {
            if (level.get_cell(i, j) == 'R') return {static_cast<float>(j), static_cast<float>(i)};
        }
    }

    for (int i = 0; i < level.get_rows(); i++) { // if forgot to add spawn
        for (int j = 0; j < level.get_columns(); j++) {
            if (level.get_cell(i, j) == 'L' || level.get_cell(i, j) == 'U' || level.get_cell(i, j) == 'D') return {static_cast<float>(j), static_cast<float>(i)};
        }
    }
    return {0,0};
}

void Player::increment_score() {
    PlaySound(coin_sound);
    level_score++;
}