#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "constants/physics.h"

class Player
{
public:
    static Player& get_instance()
    {
        static Player instance;
        return instance;
    }

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&) = delete;
    Player& operator=(Player&) = delete;

    void reset_stats();
    void spawn(float pos_x, float pos_y);
    void kill();
    void move_horizontally(float delta);
    void update();
    void update_gravity();
    void out_of_bounds();
    void increment_lifes() { lives++; }

    [[nodiscard]] Vector2 get_position() const {return position;}
    [[nodiscard]] float get_y_velocity() const {return y_velocity;}
    [[nodiscard]]int get_lives() const {return lives;}
    [[nodiscard]]bool get_is_on_ground() const {return is_on_ground;}
    [[nodiscard]]bool get_is_looking_forward() const {return is_looking_forward;}
    [[nodiscard]]bool get_is_moving() const {return is_moving;}
    [[nodiscard]] Vector2 get_spawn_pos() const;
    [[nodiscard]] int get_score() const {return level_score;}

    void set_y_velocity(float y_velocity) {this->y_velocity = y_velocity;}
    void set_on_ground(bool on_ground) { is_on_ground = on_ground; }
    void set_moving(bool moving) { is_moving = moving; }

private:
    Player();
    ~Player() = default;
    void increment_score();

    float y_velocity;
    Vector2 position;
    bool is_on_ground;
    bool is_looking_forward;
    bool is_moving;
    int init_player_pos_x, init_player_pos_y;
    int level_score;
    int lives = physics::START_LIFES;
    char current_bound;
};

#endif //PLAYER_H