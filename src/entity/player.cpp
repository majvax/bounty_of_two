#include "player.hpp"

Player::Player(float x, float y, Color color)
    : position({ x, y }), color(color) {
}

void Player::update(float deltaTime) {
    if (IsKeyDown(KEY_RIGHT)) position.x += stats.GetSpeed() * deltaTime;
    if (IsKeyDown(KEY_LEFT))  position.x -= stats.GetSpeed() * deltaTime;
    if (IsKeyDown(KEY_UP))    position.y -= stats.GetSpeed() * deltaTime;
    if (IsKeyDown(KEY_DOWN))  position.y += stats.GetSpeed() * deltaTime;
}

void Player::draw() const {
    DrawRectangle(
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(stats.GetSize()),
        static_cast<int>(stats.GetSize()),
        color
    );
}

Vector2 Player::GetPosition() const {
    return position;
}

Vector2 Player::GetCenter() const {
    return { position.x + stats.GetSize() / 2.0f, position.y + stats.GetSize() / 2.0f };
}

void Player::TakeDamage() {
    stats.SetLife(stats.GetLife() - 1) ;
}

bool Player::IsDead() const {
    return stats.GetLife() <= 0;
}

void Player::Reset(float x, float y) {
    position = { x, y };
    stats.SetLife(5) ;
}
