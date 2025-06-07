#pragma once
#include "raylib.h"

class player {
private:
    Vector2 position;
    float speed;
    float size;
    Color color;
    int health;

public:
    player(float x, float y, float speed = 200.0f, float size = 40.0f, Color color = BLUE);

    void update(float deltaTime) override;
    void draw() const override;
    Vector2 GetPosition() const;
    float GetSize() const;

    void TakeDamage(int amount);
    bool IsDead() const;
    int GetHealth() const;

    void Reset(float x, float y);
};