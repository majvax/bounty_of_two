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
    Player(float x, float y, float speed = 200.0f, float size = 40.0f, Color color = BLUE);

    void update(float deltaTime) override;
    void draw() const override;
    Vector2 GetPosition() const;
    Vector2 GetCenter() const;

    void TakeDamage(int amount);
    bool IsDead() const;


    GETTER_SETTER(float, Speed, speed)
    GETTER_SETTER(int, Health, health)
    GETTER_SETTER(float, Size, size)
    GETTER_SETTER(float, CritChance, crit_chance)
    GETTER_SETTER(float, CritMultiplier, crit_multiplier)

    PlayerStats& getStats() { return stats; }


    void Reset(float x, float y);
};
