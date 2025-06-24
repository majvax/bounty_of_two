#pragma once
#include "enemy_stats.hpp"
#include <raylib-cpp.hpp>

class GameState;
class Player;

class EntityBase {
public:
    virtual ~EntityBase() = default;    
    virtual void update(float deltatime) = 0;
    virtual void draw() const = 0;
};


class EnemyBase : public EntityBase {
public:
    virtual ~EnemyBase() = default;
    virtual void SetTarget(Player* player) = 0;
    virtual Vector2 GetPosition() const = 0;
    virtual Vector2 GetCenter() const = 0;
    virtual EnemyStats& getStats() = 0;
    virtual void MarkAsDead() = 0;
    virtual bool IsDead() const = 0;
};
