#pragma once
#include "raylib.h"
#include "Player.h"
#include "base.hpp"
class Player;

class EnemySlayer : public EntityBase {
private:
    Vector2 position;
    float speed;
    float size;
    Color color;
    bool dead;

public:
    EnemySlayer(int screenWidth, int screenHeight, float speed = 100.0f, float size = 30.0f, Color color = RED);

    void update(float deltaTime, const Player& player) override;
    void draw() const override;
    Vector2 GetPosition() const;
    float GetSize() const;

    void MarkAsDead();
    bool IsDead() const;
};