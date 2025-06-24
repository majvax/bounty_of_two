#pragma once
#include <raylib-cpp.hpp>
#include "base.hpp"
#include "enemy_stats.hpp"

class Player;

class EnemySlayer : public EnemyBase {
private:
    Vector2 upper_left_bound;
    Vector2 position;
    Color color;
    bool dead;
    Player* player;
    EnemyStats stats;
    GameState* game_state;
    Texture2D sprite;

public:
    EnemySlayer(GameState* game_state,Vector2 upper_left_bound, int screenWidth, int screenHeight, raylib::Color color = raylib::Color::Red());

    void SetTarget(Player* player) override;
    void update(float deltaTime) override;
    void draw() const override;
    Vector2 GetPosition() const override;
    Vector2 GetCenter() const override;

    void MarkAsDead() override;
    bool IsDead() const override;
    void CheckCollisionWith(const EnemySlayer* other);
    EnemyStats& getStats() override { return stats; }
};
