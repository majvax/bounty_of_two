#pragma once
#include "base.hpp"
#include "raylib.h"

class Player : public EntityBase {
private:
    Vector2 position;
    float speed;
    float size;
    Color color;
    int health;
    float crit_chance;
    float crit_multiplier;
public:
    Player(float x, float y, float speed = 200.0f, float size = 40.0f, Color color = BLUE);

    void update(float deltaTime) override;
    void draw() const override;
    Vector2 GetPosition() const;
    Vector2 GetCenter() const;
    float GetSize() const;

    void TakeDamage(int amount);
    bool IsDead() const;
    int GetHealth() const;
    float GetSpeed() const;
    float GetCritChance() const;
    float GetCritMultiplier() const;

    void SetSpeed(float new_speed);
    void SetHealth(int new_health);
    void SetSize(float new_size);
    void SetCritChance(float new_chance);
    void SetCritMultiplier(float new_multiplier);

    void Reset(float x, float y);
};
