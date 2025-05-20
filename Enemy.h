#ifndef ENEMY_H
#define ENEMY_H

#include "globals.h"

class Enemy
{
public:
    static Enemy& get_instance()
    {
        static Enemy instance;
        return instance;
    }

    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;
    Enemy(Enemy&) = delete;
    Enemy& operator=(Enemy&) = delete;

    void spawn();
    void update();
    bool is_colliding(Vector2 pos);
    void remove_colliding(Vector2 pos);
    void draw() const;

private:
    Enemy() = default;
    ~Enemy() = default;
    struct enemy_data {
        Vector2 pos;
        bool is_looking_right;
    };

    std::vector<enemy_data> enemies;
};

#endif //ENEMY_H
