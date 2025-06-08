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

public:
    Player(float x, float y, float speed = 200.0f, float size = 40.0f, Color color = BLUE);

    void process(float deltaTime);
    void draw() const;
    Vector2 GetPosition() const;
    float GetSize() const;

    void TakeDamage(int amount);
    bool IsDead() const;
    int GetHealth() const;

    void Reset(float x, float y);
};