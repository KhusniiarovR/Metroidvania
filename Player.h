#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "globals.h"

class Player
{
public:
    Player();

    void reset_stats();
    void spawn(float pos_x, float pos_y);
    void kill();
    void move_horizontally(float delta);
    void update();
    void update_gravity();
    void out_of_bounds();

    [[nodiscard]] Vector2 get_position() const {return position;}
    [[nodiscard]] float get_y_velocity() const {return y_velocity;}
    void set_y_velocity(float y_velocity) {this->y_velocity = y_velocity;}
    [[nodiscard]]int get_lives() const {return lives;}
    [[nodiscard]]bool get_is_on_ground() const {return is_on_ground;}
    void set_on_ground(bool on_ground) { is_on_ground = on_ground; }
    [[nodiscard]]bool get_is_looking_forward() const {return is_looking_forward;}
    [[nodiscard]]bool get_is_moving() const {return is_moving;}
    void set_moving(bool moving) { is_moving = moving; }
    [[nodiscard]] int get_total_score() const;
    [[nodiscard]] Vector2 get_player_pos() const;

private:
    void increment_score();

    float y_velocity;
    Vector2 position;
    bool is_on_ground;
    bool is_looking_forward;
    bool is_moving;

    int init_player_pos_x, init_player_pos_y = 1;

    int level_scores[3];

    const int MAX_LIVES = 3;
    int lives = MAX_LIVES;

    char current_bound = 'L';
};



#endif //PLAYER_H
