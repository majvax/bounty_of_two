#pragma once
#include "player.hpp"
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
    EnemySlayer(int screenWidth, int screenHeight, float speed = 100.0f, float size = 30.0f, Color color = RED);

    void SetTarget(Player* player);
    void process(float deltaTime);
    void draw() const;
    Vector2 GetPosition() const;
    float GetSize() const;

    void MarkAsDead();
    bool IsDead() const;
};
