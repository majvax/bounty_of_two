#pragma once
#include <raylib.h>
#include <vector>
#include "base.hpp"
#include "player_stats.hpp"
#include "player_bullet.hpp"

enum Direction {
    DOWN,
    LEFT,
    UP,
    RIGHT,
};

class Player : public EntityBase {
private:
    GameState* game_state;
    Vector2 position;
    Vector2 velocity;
    Color color;
    PlayerStats stats;
    std::vector<Texture2D> sprites;
    int current_sprite;
    Direction direction;
    float frame_timer;
    bool flip_h;
    std::vector<PlayerBullet*> bullets;
    float invincibility_timer;
    float shoot_timer;
    Texture2D aim_sprite;

public:
    Player(GameState* game_state, float x, float y, Color color = BLUE);

    void update(float deltaTime) override;
    void draw() const override;
    Vector2 GetPosition() const;
    Vector2 GetCenter() const;
    Vector2 GetVelocity() const;

    void TakeDamage();
    bool IsDead() const;

    void Fling(Vector2 direction);

    PlayerStats& GetStats() { return stats; }
    void ShootBullet(Vector2 direction);
    void RemoveBullet(PlayerBullet* bullet);

    void Reset(float x, float y);
};
