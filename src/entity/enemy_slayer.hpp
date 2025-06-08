#pragma once
#include <raylib-cpp.hpp>
#include "base.hpp"

class Player;

class EnemySlayer : public EntityBase {
private:
    Vector2 position;
    float speed;
    float size;
    Color color;
    bool dead;
    Player* player;

public:
    EnemySlayer(int screenWidth, int screenHeight, float speed = 100.0f, float size = 30.0f, raylib::Color color = raylib::Color::Red());

    void SetTarget(Player* player);
    void update(float deltaTime) override;
    void draw() const override;
    Vector2 GetPosition() const;
    float GetSize() const;

    void MarkAsDead();
    bool IsDead() const;
};
