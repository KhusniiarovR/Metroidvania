#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"

class Level {
public:
    static Level& get_instance()
    {
        static Level instance;
        return instance;
    }

    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;
    Level(Level&) = delete;
    Level& operator=(Level&) = delete;

    [[nodiscard]] bool is_inside(int row, int column) const;
    bool is_colliding(Vector2 pos, char look_for = '#');
    char& get_collider(Vector2 pos, char look_for);

    void reset_index();
    void load_level(int offset = 0);
    void reset_data(const std::string& source_path = "data/saves/levels_copy.rll", const std::string& dest_path = "data/saves/levels.rll");

    [[nodiscard]] char& get_cell(size_t row, size_t column);
    [[nodiscard]] int get_rows() const;
    [[nodiscard]] int get_columns() const;
    [[nodiscard]] int get_index() const;
    [[nodiscard]] int get_bounds(int dir) const;

    void set_cell(size_t row, size_t column, char chr);

private:
    Level();
    ~Level() = default;

    int rows, columns;
    std::string data;
    int level_index = 1;
    int index_to_bounds[4]{0,0,0,0};

    void decode_file();
    std::string calculate_level_size(const std::string& nextLine);
    void encrypt(const std::string& filename = "data/saves/levels.rll");
};

#endif //LEVEL_H
