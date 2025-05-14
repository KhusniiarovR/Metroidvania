#ifndef ENEMY_H
#define ENEMY_H

#include "globals.h"
#include "player.h"

class Enemy
{
public:

    void spawn();
    void update();
    bool is_colliding_enemy(Vector2 pos);
    void remove_colliding(Vector2 pos);
    void draw() const;

    [[nodiscard]] size_t get_enemy_count() const { return enemies.size(); }

private:
    struct enemy_data {
        Vector2 pos;
        bool is_looking_right;
    };

    std::vector<enemy_data> enemies;
};

#endif //ENEMY_H
