#include "enemy_slayer.hpp"
#include "../scene/game_state.hpp"
#include "player.hpp"
#include <cmath>
#include <raymath.h>

EnemySlayer::EnemySlayer(GameState* game_state, int screenWidth, int screenHeight, raylib::Color color)
    : color(color), dead(false), player(nullptr), game_state(game_state)
{
    int side = GetRandomValue(0, 3);

    switch (side) {
    case 0:
        position = { (float)GetRandomValue(0, screenWidth), -stats.GetSize()};
        break;
    case 1:
        position = { (float)GetRandomValue(0, screenWidth), (float)screenHeight + stats.GetSize()};
        break;
    case 2:
        position = { -stats.GetSize(), (float)GetRandomValue(0, screenHeight)};
        break;
    case 3:
        position = { (float)screenWidth + stats.GetSize(), (float)GetRandomValue(0, screenHeight)};
        break;
    }
}

void EnemySlayer::SetTarget(Player* player) {
    this->player = player;
}

void EnemySlayer::update(float deltaTime) {
    if (IsDead()) return;
    Vector2 target = player->GetCenter();
    Vector2 center = { position.x + stats.GetSize() / 2.0f, position.y + stats.GetSize() / 2.0f };
    Vector2 direction = { target.x - center.x, target.y - center.y };

    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length <= player->GetStats().GetSize() / 1.5f) {
        player->TakeDamage();
        player->Fling(Vector2Multiply(Vector2Normalize(direction), {200, 200}));
        return; // Do not move if too close to the player
    }

    // Normalize the direction vector
    direction.x /= length;
    direction.y /= length;

    // Move the slayer towards the player
    position.x += direction.x * stats.GetSpeed() * deltaTime;
    position.y += direction.y * stats.GetSpeed() * deltaTime;
}

void EnemySlayer::draw() const {
    DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y),
        static_cast<int>(stats.GetSize()), static_cast<int>(stats.GetSize()), color);
}

Vector2 EnemySlayer::GetPosition() const {
    return position;
}


void EnemySlayer::MarkAsDead() {
    dead = true;
}

bool EnemySlayer::IsDead() const {
    return dead;
}
