#include "player_bullet.hpp"
#include "../scene/game_state.hpp"
#include "base.hpp"
#include "enemy_slayer.hpp"
#include <cstddef>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <raymath.h>

PlayerBullet::PlayerBullet(GameState* game_state, float x, float y, float vx, float vy, float hitbox_radius, int damage)
    : game_state(game_state), 
    position({x,y}),
    velocity({vx,vy}),
    hitbox_radius(hitbox_radius),
    damage(damage)
{}

void PlayerBullet::update(float deltaTime){
    position = Vector2({
        position.x + velocity.x * deltaTime,
        position.y+velocity.y*deltaTime
    });
    Rectangle this_rect {
        position.x-hitbox_radius,
        position.y-hitbox_radius,
        hitbox_radius,
        hitbox_radius
    };
    for (size_t i = 0; i < game_state->GetEntities().size(); i++) {
        EnemySlayer* entity_enemy = reinterpret_cast<EnemySlayer*>(game_state->GetEntities()[i].get());
        if (entity_enemy != nullptr){
            Vector2 enemy_pos = entity_enemy->GetPosition();
            float enemy_size = entity_enemy->getStats().GetSize();

            if (CheckCollisionRecs(
                this_rect,
                Rectangle{
                    enemy_pos.x, enemy_pos.y,
                    enemy_size,enemy_size
                }
            ))
            {
                MarkDespawn(true);
                entity_enemy->MarkAsDead();
            }
        }
    }
}

void PlayerBullet::draw() const{
    DrawCircle(position.x, position.y, hitbox_radius, BLACK);
}

void PlayerBullet::MarkDespawn(bool despawn_state){ despawn = despawn_state; }

const bool PlayerBullet::GetDespawn() const{ return despawn;}