#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include "player.h"
#include "enemy.h"


extern Enemy enemy;

class Level {
public:
    Level();
    [[nodiscard]] bool is_inside(int row, int column) const;
    bool is_colliding(Vector2 pos, char look_for = '#');
    char& get_collider(Vector2 pos, char look_for);

    void reset_index();
    void load_level(int offset = 0);

    char& get_cell(size_t row, size_t column);
    void set_cell(size_t row, size_t column, char chr);

    [[nodiscard]] int get_rows() const;
    [[nodiscard]] int get_columns() const;
    [[nodiscard]] int get_index() const;
    [[nodiscard]] int get_count() const;
private:

    int rows, columns;
    std::string data;
    int level_index = 0;
    int LEVEL_COUNT = 3;
    void decode_file();
};

#endif //LEVEL_H
