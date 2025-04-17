#include "player.h"
#include "enemy.h"
#include "globals.h"
#include "level.h"


extern Enemy enemy;
extern Level level;

Player::Player() : position({0.0f,0.0f}), y_velocity(0),
                   is_on_ground(true), is_moving(false),
                   is_looking_forward(true) {}


void Player::reset_stats() {
    lives = MAX_LIVES;
    for (int & level_score : level_scores) {
        level_score = 0;
    }
}

int Player::get_total_score() const {
    int sum = 0;

    for (const int level_score : level_scores) {
        sum += level_score;
    }

    return sum;
}

void Player::increment_score() {
    PlaySound(coin_sound);
    level_scores[level.get_index()]++;
}

void Player::spawn() {
    y_velocity = 0;

    for (size_t row = 0; row < level.get_rows(); ++row) {
        for (size_t column = 0; column < level.get_columns(); ++column) {
            char cell = level.get_cell(row, column);;

            if (cell == PLAYER) {
                position.x = column;
                position.y = row;
                level.set_cell(row, column, AIR);
                return;
            }
        }
    }
}

void Player::kill() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    lives--;
    level_scores[level.get_index()] = 0;
}

void Player::move_horizontally(float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    float next_x = position.x + delta;
    if (!level.is_colliding({next_x, position.y}, WALL)) {
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

void Player::update_gravity() {
    // Bounce downwards if approaching a ceiling with upwards velocity
    if (level.is_colliding({position.x, position.y - 0.1f}, WALL) && y_velocity < 0) {
        y_velocity = CEILING_BOUNCE_OFF;
    }

    // Add gravity to player's y-position
    position.y += y_velocity;
    y_velocity += GRAVITY_FORCE;

    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
    is_on_ground = level.is_colliding({position.x, position.y + 0.1f}, WALL);
    if (is_on_ground) {
        y_velocity = 0;
        position.y = roundf(position.y);
    }
}

void Player::update() {
    player.update_gravity();

    // Interacting with other level elements
    if (level.is_colliding(position, COIN)) {
        level.get_collider(position, COIN) = AIR; // Removes the coin
        player.increment_score();
    }

    if (level.is_colliding(position, EXIT)) {
        // Reward player for being swift
        if (timer > 0) {
            // For every 9 seconds remaining, award the player 1 coin
            timer -= 25;
            time_to_coin_counter += 5;

            if (time_to_coin_counter / 60 > 1) {
                player.increment_score();
                time_to_coin_counter = 0;
            }
        }
        else {
            // Allow the player to exit after the level timer goes to zero
            level.load(1);
            PlaySound(exit_sound);
        }
    }
    else {
        // Decrement the level timer if not at an exit
        if (timer >= 0) timer--;
    }

    // Kill the player if they touch a spike or fall below the level
    if (level.is_colliding(position, SPIKE) || position.y > level.get_rows()) {
        player.kill();
    }

    // Upon colliding with an enemy...
    if (enemy.is_colliding_enemy(position)) {
        // ...check if their velocity is downwards...
        if (y_velocity > 0) {
            // ...if yes, award the player and kill the enemy
            enemy.remove_colliding(position);
            PlaySound(kill_enemy_sound);

            player.increment_score();
            y_velocity = -BOUNCE_OFF_ENEMY;
        }
        else {
            // ...if not, kill the player
            player.kill();
        }
    }
}

