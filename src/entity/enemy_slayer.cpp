#include "enemy_slayer.hpp"
#include "../scene/game_state.hpp"
#include "player.hpp"
#include <cmath>
#include <raymath.h>

EnemySlayer::EnemySlayer(GameState* game_state, Vector2 upper_left_bound, int screenWidth, int screenHeight, raylib::Color color)
    : upper_left_bound(upper_left_bound), color(color), dead(false), player(nullptr), stats(), game_state(game_state), sprite(LoadTexture("assets/enemy.png"))
{
    int side = GetRandomValue(0, 3);

    switch (side) {
    case 0:  // Spawn from the top
        position = { (float)GetRandomValue(upper_left_bound.x, upper_left_bound.x + screenWidth), upper_left_bound.y };
        break;
    case 1: // Spawn from the bottom
        position = { (float)GetRandomValue(upper_left_bound.x, upper_left_bound.x + screenWidth), upper_left_bound.y + screenHeight };
        break;
    case 2: // Spawn from the left
        position = { upper_left_bound.x, (float)GetRandomValue(upper_left_bound.y, upper_left_bound.y + screenHeight) };
        break;
    case 3: // Spawn from the right
        position = { upper_left_bound.x + screenWidth, (float)GetRandomValue(upper_left_bound.y, upper_left_bound.y + screenHeight) };
        break;
    }
}

void EnemySlayer::SetTarget(Player* player) {
    this->player = player;
}

void EnemySlayer::CheckCollisionWith(EnemySlayer* other){
    if (Vector2Distance(other->GetPosition(), position) < getStats().GetSize()){
        position = Vector2Subtract(position, Vector2Multiply(Vector2Subtract(other->GetPosition(), position), {0.1,0.1}));
    }
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
    
    for (int i = 0; i < game_state->GetEntities().size(); i++){
        if (game_state->GetEntities()[i].get() != this){
            CheckCollisionWith(reinterpret_cast<EnemySlayer*>(game_state->GetEntities()[i].get()));
        }
    }
}

void EnemySlayer::draw() const {
    // DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y),
    //     static_cast<int>(stats.GetSize()), static_cast<int>(stats.GetSize()), color);
    DrawTexturePro(
        sprite,
        {0,0,(float)sprite.width, (float)sprite.height},
        {GetCenter().x,GetCenter().y,sprite.width*stats.GetSize()/40.0f,sprite.height*stats.GetSize()/40.0f},
        {sprite.width/2.0f,sprite.height/2.0f}, 0.0, WHITE
    );
}

Vector2 EnemySlayer::GetPosition() const {
    return position;
}

Vector2 EnemySlayer::GetCenter() const {
    return { position.x + stats.GetSize() / 2.0f, position.y + stats.GetSize() / 2.0f };
}


void EnemySlayer::MarkAsDead() {
    dead = true;
}

bool EnemySlayer::IsDead() const {
    return dead;
}
