#pragma once
#include "base.hpp"
#include "raylib.h"
#include "../utils/getter_setter.hpp"
#include "player_stats.hpp"

class Player : public EntityBase {
private:
    Vector2 position;
    Color color;
    PlayerStats stats;

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
