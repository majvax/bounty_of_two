#include "player.hpp"
#include "../scene/game_state.hpp"
#include "enemy_slayer.hpp"
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cfloat>

Player::Player(GameState* game_state, float x, float y, Color color)
    : game_state(game_state), position({ x, y }), velocity({0,0}), color(color), stats(), sprites(), current_animation_frame(0),
    invincibility_timer(0), direction(DOWN), frame_timer(0), flip_h(false), shoot_timer(0.5),
    aim_sprite(LoadTexture("assets/direction.png")), bullets(), target(nullptr) {
    for (size_t i = 0; i < 11; i++) {
        Texture2D image = LoadTexture(("assets/crocoimages/image" + std::to_string(i) + ".png").c_str());
        sprites.push_back(image);
    }
}

void Player::update(float deltaTime) {
    if (IsDead()) return;
    invincibility_timer -= deltaTime;
    shoot_timer -= deltaTime;

    Vector2 target_velocity = Vector2();
    if (IsKeyDown(KEY_RIGHT)) {
        target_velocity.x += 1;
        direction = RIGHT;
    }
    if (IsKeyDown(KEY_LEFT))  {
        target_velocity.x -= 1;
        direction = LEFT;
    }
    if (IsKeyDown(KEY_UP)) {
        target_velocity.y -= 1;
        direction = UP;
    }
    if (IsKeyDown(KEY_DOWN)) {
        target_velocity.y += 1;
        direction = DOWN;
    }
    target_velocity = Vector2Multiply(Vector2Normalize(target_velocity), Vector2({stats.GetSpeed(), stats.GetSpeed()}));
    // Normalizing movement prevents an unintentional speed boost from going diagonally
    velocity = Vector2MoveTowards(velocity, target_velocity, deltaTime*stats.GetAcceleration());
    position.x += velocity.x*deltaTime;
    position.y += velocity.y*deltaTime;

    // update sprite
    if (Vector2LengthSqr(velocity) > 1){
        frame_timer -= deltaTime;
        if (frame_timer <= 0){
            frame_timer = 0.1;
            current_animation_frame += 1;
        }
        if (direction == LEFT or direction == RIGHT){
            if (current_animation_frame > 2) current_animation_frame = 0;
            flip_h = (direction == LEFT);
        }else{
            if (current_animation_frame > 3) current_animation_frame = 0;
            flip_h = false;
        }
    }else{
        current_animation_frame = 0;
        flip_h = false;
    }    // Find closest enemy and set as target
    float closest_distance = FLT_MAX;
    EnemyBase* closest_enemy = nullptr;
    
    for (const auto& enemy : game_state->GetEntities()) {
        if (enemy && !enemy->IsDead()) {
            float distance = Vector2Distance(GetCenter(), enemy->GetPosition());
            if (distance < closest_distance) {
                closest_distance = distance;
                closest_enemy = enemy.get();
            }
        }
    }
    target = closest_enemy;    // Shoot bullets
    if (Vector2LengthSqr(target_velocity) < 0.1 && shoot_timer < 0.0 && target && !target->IsDead()){        // Validate target is still in game state before using it
        bool target_valid = false;
        for (const auto& enemy : game_state->GetEntities()) {
            if (enemy.get() == target.get() && !enemy->IsDead()) {
                target_valid = true;
                break;
            }
        }
        
        if (target_valid) {
            Vector2 target_direction = Vector2Subtract(target->GetCenter(), GetCenter());
            Vector2 look_direction = Vector2Normalize(target_direction);
            ShootBullet(look_direction);            shoot_timer = 0.5;
        } else {
            target = static_cast<EnemyBase*>(nullptr); // Clear invalid target
        }
    }else if (Vector2LengthSqr(target_velocity) > 0.1){
        shoot_timer = 0.5;
    }// Bullet management
    for (auto it = bullets.begin(); it != bullets.end();) {
        auto* bullet = *it;
        bullet->update(deltaTime);
        if (Vector2Distance(GetPosition(), bullet->GetPosition()) > 2048 || bullet->GetDespawn()) {
            delete bullet;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}

void Player::draw() const {
    int current_sprite = GetFrame();
    Rectangle rect = Rectangle({
        1.0, 1.0, 
        static_cast<float>(sprites[current_sprite].width),
        static_cast<float>(sprites[current_sprite].height)
    });
    rect.width *= flip_h ? -1.0 : 1.0;
    DrawTextureRec( // draw the current sprite centered on the center position
        sprites[current_sprite],
        rect,
        Vector2Subtract(GetCenter(), Vector2({
            static_cast<float>(sprites[current_sprite].width/2.0),
            static_cast<float>(sprites[current_sprite].height/2.0)
        })), WHITE
    );
    Vector2 look_direction;
    if (target) {
        // Validate target is still in game state before using it
        bool target_valid = false;
        for (const auto& enemy : game_state->GetEntities()) {
            if (enemy.get() == target.get() && !enemy->IsDead()) {
                target_valid = true;
                break;
            }
        }
        
        if (target_valid) {
            Vector2 target_direction = Vector2Subtract(target->GetCenter(), GetCenter());            
            look_direction = Vector2Multiply(Vector2Normalize(target_direction), Vector2({64,64}));
        } else {
            target = nullptr; // Clear invalid target
            Vector2 relative_mouse_position = Vector2({
                static_cast<float>(GetMouseX()-GetScreenWidth()/2.0),
                static_cast<float>(GetMouseY()-GetScreenHeight()/2.0),
            });
            look_direction = Vector2Multiply(Vector2Normalize(relative_mouse_position), Vector2({64,64}));
        }
    } else {
        Vector2 relative_mouse_position = Vector2({
            static_cast<float>(GetMouseX()-GetScreenWidth()/2.0),
            static_cast<float>(GetMouseY()-GetScreenHeight()/2.0),
        });
        look_direction = Vector2Multiply(Vector2Normalize(relative_mouse_position), Vector2({64,64}));
    }

    for (const auto* bullet : bullets) {
        bullet->draw();
    }

    // Draw aim
    float rotation = 0.0f;
    if (Vector2LengthSqr(look_direction) > 0.0f) {
        rotation = -Vector2Angle(look_direction, {0,-1})*180.0/3.1415926535;
    }
    DrawTexturePro(aim_sprite, {0,0,64,64}, 
        {look_direction.x+GetCenter().x,
        look_direction.y+GetCenter().y,64,64},
        {32,32}, rotation, WHITE);
}

int Player::GetFrame() const {
    if (Vector2LengthSqr(velocity) > 0.1){
        const std::vector<std::vector<int>> frames_id = {
            {0,9,0,10},
            {4,5,6},
            {2,7,2,8},
            {4,5,6},
        };
        return frames_id[(int)direction][current_animation_frame];
    }
    return (int)direction;
}

Vector2 Player::GetPosition() const {
    return position;
}

Vector2 Player::GetCenter() const {
    return { position.x + stats.GetSize() / 2.0f, position.y + stats.GetSize() / 2.0f };
}

Vector2 Player::GetVelocity() const {
    return velocity;
}

void Player::TakeDamage() {
    if (invincibility_timer > 0.0) return;
    invincibility_timer = 1.0;
    stats.SetLife(stats.GetLife() - 1) ;
}

void Player::Fling(Vector2 direction){
    velocity.x += direction.x;
    velocity.y += direction.y;
}

bool Player::IsDead() const {
    return stats.GetLife() <= 0;
}

void Player::Reset(float x, float y) {
    position = { x, y };
    stats.SetLife(5) ;
}

void Player::ShootBullet(Vector2 direction){
    Rectangle bullet_hitbox{0,0,64,64};
    Vector2 bullet_velocity = Vector2Multiply(Vector2Normalize(direction), {300,300});
    PlayerBullet* bullet = new PlayerBullet(
        game_state,
        GetCenter().x, GetCenter().y,
        bullet_velocity.x, bullet_velocity.y,
        16, stats.GetDamage()
    );
    bullets.push_back(bullet);
}


void Player::RemoveBullet(PlayerBullet* bullet)
{
    auto it = std::remove_if(bullets.begin(), bullets.end(),
        [bullet](const PlayerBullet* e) { return e == bullet; });
    bullets.erase(it, bullets.end());
}
