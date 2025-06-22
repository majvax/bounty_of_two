#pragma once
#include <raylib-cpp.hpp>
#include "base.hpp"
#include "enemy_stats.hpp"

class Player;

class EnemySlayer : public EntityBase {
private:
    GameState* game_state;
    Vector2 position;
    Color color;
    bool dead;
    Player* player;
    EnemyStats stats;

public:
    EnemySlayer(GameState* game_state, int screenWidth, int screenHeight, raylib::Color color = raylib::Color::Red());

    void SetTarget(Player* player);
    void update(float deltaTime) override;
    void draw() const override;
    Vector2 GetPosition() const;
    float GetSize() const;

    void MarkAsDead();
    bool IsDead() const;
    EnemyStats& getStats() { return stats; }
};
