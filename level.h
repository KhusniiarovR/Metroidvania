#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include "player.h"
#include "enemy.h"

extern Player player;
extern Enemy enemy;

bool is_inside_level(int row, int column) {
    if (row < 0 || row >= current_level.rows) return false;
    if (column < 0 || column >= current_level.columns) return false;
    return true;
}

bool is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    // Scan the adjacent area in the level to look for a match in collision
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char& get_collider(Vector2 pos, char look_for) {
    // Like is_colliding(), except returns a reference to the colliding object
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return get_level_cell(row, column);
                }
            }
        }
    }

    // If failed, get an approximation
    return get_level_cell(pos.x, pos.y);;
}

void reset_level_index() {
    level_index = 0;
}

void load_level(int offset) {
    level_index += offset;

    // Win logic
    if (level_index >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    // Level duplication
    size_t rows = LEVELS[level_index].rows;
    size_t columns = LEVELS[level_index].columns;
    current_level_data = new char[rows*columns];

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            current_level_data[row * columns + column] = LEVELS[level_index].data[row * columns + column];
        }
    }

    current_level = {rows, columns, current_level_data};

    // Instantiate entities
    player.spawn();
    enemy.spawn();

    // Calculate positioning and sizes
    derive_graphics_metrics_from_loaded_level();

    // Reset the timer
    timer = MAX_LEVEL_TIME;
}

void unload_level() {
    delete[] current_level_data;
}

// Getters and setters
char& get_level_cell(size_t row, size_t column) {
    return current_level.data[row * current_level.columns + column];
}

void set_level_cell(size_t row, size_t column, char chr) {
    get_level_cell(row, column) = chr;
}

#endif //LEVEL_H
