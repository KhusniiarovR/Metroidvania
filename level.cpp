#include "level.h"
#include <fstream>
#include <iostream>

extern Player player;

Level::Level() : rows(0), columns(0) {}

bool Level::is_inside(int row, int column) const {
    if (row < 0 || row >= rows) return false;
    if (column < 0 || column >= columns) return false;
    return true;
}

bool Level::is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    // Scan the adjacent area in the level to look for a match in collision
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside(row, column)) continue;
            if (get_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char& Level::get_collider(Vector2 pos, char look_for) {
    // Like is_colliding(), except returns a reference to the colliding object
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside(row, column)) continue;
            if (get_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return get_cell(row, column);
                }
            }
        }
    }

    // If failed, get an approximation
    return get_cell(pos.x, pos.y);;
}

void Level::reset_index() {
    level_index = 0;
}

void Level::load_level(int offset) {
    level_index += offset;

    // Win logic
    if (level_index >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    decode_file();

    // Instantiate entities
    player.spawn();
    enemy.spawn();

    // Calculate positioning and sizes
    derive_graphics_metrics_from_loaded_level();

    // Reset the timer
    timer = MAX_LEVEL_TIME;
}

void Level::decode_file() {
    data = nullptr;
    rows = 1;
    columns = 0;

    std::ifstream level_file("data/levels.rll");

    if (!level_file) {
        std::cerr << "can't open levels.rll!" << std::endl;
    }

    std::string line;
    std::string target_level = "; Level " + std::to_string(level_index + 1);
    while (std::getline(level_file, line)) {
        if (line == target_level) {
            break;
        }
    }

    std::string nextLine;
    std::getline(level_file, nextLine);

    std::string decoded;
    size_t i = -1;
    int columns_number = 0;

    while (i < nextLine.length()) {
        ++i;
        if (isdigit(nextLine[i])) {
            int num = 0;
            while (i < nextLine.length() && isdigit(nextLine[i])) {
                num = num * 10 + (nextLine[i] - '0');
                ++i;
            }
            if (i < nextLine.length()) {
                char symbol = nextLine[i++];
                decoded.append(num, symbol);
                columns_number += num;
                ++i;
                continue;
            }
        }

        else if (nextLine[i] == '|') {
            columns_number = 0;
            rows++;
            ++i;
            continue;
        }

        decoded.push_back(nextLine[i]);
        columns_number++;

        columns = columns_number;
    }
    // todo исправь декодинг весь
    std::cerr << decoded;

    std::cout << "\n" << rows << "\n" << columns << "\n";
    strcpy(data, decoded.c_str());

    level_file.close();
}

char& Level::get_cell(size_t row, size_t column) {
    return data[row * columns + column];
}

void Level::set_cell(size_t row, size_t column, char chr) {
    get_cell(row, column) = chr;
}

int Level::get_rows() const {
    return rows;
}

int Level::get_columns() const {
    return columns;
}

int Level::get_index() const {
    return level_index;
}

int Level::get_count() const {
    return LEVEL_COUNT;
}