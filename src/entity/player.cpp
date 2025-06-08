#include "player.hpp"

Player::Player(float x, float y, float speed, float size, Color color)
    : position{ x, y }, speed(speed), size(size), color(color), health(5), 
      crit_chance(0.1f), crit_multiplier(2.0f) {
}

void Player::update(float deltaTime) {
    if (IsKeyDown(KEY_RIGHT)) position.x += speed * deltaTime;
    if (IsKeyDown(KEY_LEFT))  position.x -= speed * deltaTime;
    if (IsKeyDown(KEY_UP))    position.y -= speed * deltaTime;
    if (IsKeyDown(KEY_DOWN))  position.y += speed * deltaTime;
}

void Player::draw() const {
    DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y),
        static_cast<int>(size), static_cast<int>(size), color);
}

Vector2 Player::GetPosition() const {
    return position;
}

Vector2 Player::GetCenter() const {
    return { position.x + size / 2.0f, position.y + size / 2.0f };
}

float Player::GetSize() const {
    return size;
}

void Player::TakeDamage(int amount) {
    health -= amount;
}

bool Player::IsDead() const {
    return health <= 0;
}

int Player::GetHealth() const {
    return health;
}
float Player::GetSpeed() const {
    return speed;
}

float Player::GetCritChance() const {
    return crit_chance;
}

float Player::GetCritMultiplier() const {
    return crit_multiplier;
}

void Player::SetSpeed(float new_speed) {
    speed = new_speed;
}

void Player::SetHealth(int new_health) {
    health = new_health;
}

void Player::SetSize(float new_size) {
    size = new_size;
}

void Player::SetCritChance(float new_chance) {
    crit_chance = new_chance;
}

void Player::SetCritMultiplier(float new_multiplier) {
    crit_multiplier = new_multiplier;
}


void Player::Reset(float x, float y) {
    position = { x, y };
    health = 5;
}
