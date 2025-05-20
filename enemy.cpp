#include "enemy.h"
#include "raylib.h"
#include "globals.h"
#include "level.h"
#include "constants/game_elements.h"
#include "constants/physics.h"
#include "constants/graphics.h"

extern Enemy enemy;
extern Level level;
extern Player player;

void Enemy::spawn() {
    // Create enemies, incrementing their amount every time a new one is created
    enemies.clear();

    for (size_t row = 0; row < level.get_rows(); ++row) {
        for (size_t column = 0; column < level.get_columns(); ++column) {
            char cell = level.get_cell(row, column);

            if (cell == game_elements::ENEMY) {
                // Instantiate and add an enemy to the level
                enemy_data new_enemy = { {static_cast<float>(column), static_cast<float>(row)}, true };
                enemies.push_back(new_enemy);

                //level.set_cell(row, column, game_elements::AIR);
            }
        }
    }
}

void Enemy::update() {
    for (auto &enemy : enemies) {
        // Find the enemy's next x
        float next_x = enemy.pos.x;
        next_x += (enemy.is_looking_right ? physics::ENEMY_MOVEMENT_SPEED : -physics::ENEMY_MOVEMENT_SPEED);

        // If its next position collides with a wall, turn around
        if (level.is_colliding({next_x, enemy.pos.y}, game_elements::WALL)) {
            enemy.is_looking_right = !enemy.is_looking_right;
        }
        // Otherwise, keep moving
        else {
            enemy.pos.x = next_x;
        }
    }
}

bool Enemy::is_colliding_enemy(Vector2 pos) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (auto &enemy : enemies) {
        Rectangle enemy_hitbox = {(float) enemy.pos.x, (float) enemy.pos.y, 1.0f, 1.0f};
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            return true;
        }
    }
    return false;
}

void Enemy::remove_colliding(Vector2 pos) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (auto it = enemies.begin(); it != enemies.end(); it++) {
        Rectangle enemy_hitbox = {(float) it->pos.x, (float) it->pos.y, 1.0f, 1.0f};
        // Erase a colliding enemy
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            enemies.erase(it);

            // Call the function again to remove any remaining colliding enemies
            // Cannot continue as calling erase on a vector invalidates current iterators
            enemy.remove_colliding(pos);
            return;
        }
    }
}

void Enemy::draw() const {
    float horizontal_shift = (graphics::screen_size.x - graphics::cell_size) / 2;

    for (const auto& enemy : enemies) {
        Vector2 draw_pos = {
            (enemy.pos.x - player.get_position().x) * graphics::cell_size + horizontal_shift,
            enemy.pos.y * graphics::cell_size
        };
        draw_sprite(enemy_walk, draw_pos, graphics::cell_size);
    }
}