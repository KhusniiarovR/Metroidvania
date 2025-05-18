#include "level.h"
#include <fstream>
#include <iostream>
#include <sstream>

extern Player player;

Level::Level() : rows(0), columns(0) {}

bool Level::is_inside(const int row, const int column) const {
    if (row < 0 || row >= rows) return false;
    if (column < 0 || column >= columns) return false;
    return true;
}

bool Level::is_colliding(const Vector2 pos, const char look_for) {
    const Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    // Scan the adjacent area in the level to look for a match in collision
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside(row, column)) continue;
            if (get_cell(row, column) == look_for) {
                Rectangle block_hitbox = {static_cast<float>(column), static_cast<float>(row), 1.0f, 1.0f};
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
    level_index = 1;
}

void Level::load_level(int offset) {
    if (offset == 0) return;

    level_index = offset;

    if (level_index == -1) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        reset_index();
        return;
    }

    data.clear();
    rows = 1;
    columns = 0;

    decode_file();

    // Calculate positioning and sizes
    derive_graphics_metrics_from_loaded_level();

    // Reset the timer
    timer = MAX_LEVEL_TIME;
}

std::string Level::calculate_level_size(const std::string& nextLine) {

    std::string decoded;
    long long i = 0;
    int columns_number = 0;

    while (i < nextLine.length()) {
        if (isdigit(nextLine[i])) {
            int num = 0;
            while (i < nextLine.length() && isdigit(nextLine[i])) {
                num = num * 10 + (nextLine[i] - '0');
                ++i;
            }
            if (i < nextLine.length()) {
                char symbol = nextLine[i++];
                if (symbol == 'L' || symbol == 'R' || symbol == 'U' || symbol == 'D') {
                    continue;
                }
                columns_number += num;
                decoded.append(num, symbol);
                continue;
            }
        }

        else if (nextLine[i] == '|') {
            columns = columns_number;
            columns_number = 0;
            rows++;
            ++i;
            continue;
        }

        decoded.push_back(nextLine[i]);
        columns_number++;
        i++;
    }

    return decoded;
}

void Level::decode_file() {

    std::ifstream level_file("data/levels.rll");

    if (!level_file) {
        std::cerr << "can't open levels.rll!" << std::endl;
    }

    std::string line;
    std::string target_level = "; Level " + std::to_string(level_index);
    while (std::getline(level_file, line)) {
        if (line == target_level) {
            break;
        }
    }

    std::string nextLine;
    std::getline(level_file, nextLine);

    size_t pos = nextLine.find("::");
    if (pos == std::string::npos) {
        std::cerr << "level line, missing '::'" << std::endl;
        return;
    }

    std::string level_data_str = nextLine.substr(0, pos);
    std::string bounds_data_str = nextLine.substr(pos + 2);

    data = calculate_level_size(level_data_str);

    Vector2 player_pos = player.get_player_pos();
    player.spawn(player_pos.x, player_pos.y);
    enemy.spawn();

    std::istringstream bounds_stream(bounds_data_str);
    for (int & index_to_bound : index_to_bounds) {
        bounds_stream >> index_to_bound;
    }

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

int Level::get_bounds(int dir) const {
    if (dir >= 1 && dir <= 4) {
        return index_to_bounds[dir-1];
    }
    return -1;
}
