#include "enemy_slayer.hpp"
#include <cmath>

EnemySlayer::EnemySlayer(int screenWidth, int screenHeight, float speed, float size, Color color)
    : speed(speed), size(size), color(color), dead(false)
{
    int side = GetRandomValue(0, 3);

    switch (side) {
    case 0:
        position = { (float)GetRandomValue(0, screenWidth), -size };
        break;
    case 1:
        position = { (float)GetRandomValue(0, screenWidth), (float)screenHeight + size };
        break;
    case 2:
        position = { -size, (float)GetRandomValue(0, screenHeight) };
        break;
    case 3:
        position = { (float)screenWidth + size, (float)GetRandomValue(0, screenHeight) };
        break;
    }
}

void EnemySlayer::SetTarget(Player* player) {
    this->player = player;
}

void EnemySlayer::process(float deltaTime) {
    Vector2 target = player->GetPosition();
    Vector2 direction = { target.x - position.x, target.y - position.y };

    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction.x /= length;
        direction.y /= length;
    }

    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
}

void EnemySlayer::draw() const {
    DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y),
        static_cast<int>(size), static_cast<int>(size), color);
}

Vector2 EnemySlayer::GetPosition() const {
    return position;
}

float EnemySlayer::GetSize() const {
    return size;
}

void EnemySlayer::MarkAsDead() {
    dead = true;
}

bool EnemySlayer::IsDead() const {
    return dead;
}