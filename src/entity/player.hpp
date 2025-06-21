#pragma once
#include <raylib.h>
#include <vector>
#include "base.hpp"
#include "../utils/getter_setter.hpp"
#include "player_stats.hpp"

enum Direction {
    DOWN,
    LEFT,
    UP,
    RIGHT,
};

class Player : public EntityBase {
private:
    Vector2 position;
    Vector2 velocity;
    Color color;
    PlayerStats stats;
    std::vector<Texture2D> sprites;
    int current_sprite;
    Direction direction;
    float frame_timer;
    bool flip_h;

public:
    Player(float x, float y, Color color = BLUE);

    void update(float deltaTime) override;
    void draw() const override;
    Vector2 GetPosition() const;
    Vector2 GetCenter() const;

    void TakeDamage();
    bool IsDead() const;

    PlayerStats& GetStats() { return stats; }


    void Reset(float x, float y);
};
