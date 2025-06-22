#include "player.hpp"
#include "../scene/game_state.hpp"
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <algorithm>

Player::Player(GameState* game_state, float x, float y, Color color)
    : game_state(game_state), position({ x, y }), color(color), current_sprite(0),
    invincibility_timer(0), direction(DOWN), frame_timer(0), flip_h(false) {
    for (size_t i = 0; i < 7; i++) {
        Texture2D image = LoadTexture(("assets/crocoimages/image000" + std::to_string(i) + ".png").c_str());
        sprites.push_back(image);
    }
}

void Player::update(float deltaTime) {
    invincibility_timer -= deltaTime;
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
    if (Vector2LengthSqr(velocity) > 1 && (direction == LEFT or direction == RIGHT)){
        if (current_sprite < 4 || current_sprite > 6) current_sprite = 6;
        if (frame_timer <= 0){
            frame_timer = 0.1;
            current_sprite -= 1;
            if (current_sprite == 3) current_sprite = 6;
        }
        frame_timer -= deltaTime;
        flip_h = (direction == LEFT);
    }else{
        current_sprite = (int)direction;
        flip_h = false;
    }
    // Shoot bullets
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector2 relative_mouse_position = Vector2({
            static_cast<float>(GetMouseX()-GetScreenWidth()/2.0),
            static_cast<float>(GetMouseY()-GetScreenHeight()/2.0),
        });
        Vector2 look_direction = Vector2Normalize(relative_mouse_position);
        ShootBullet(look_direction);
    }

    // Bullet management
    for (auto* bullet : bullets) {
        bullet->update(deltaTime);
        if (Vector2Distance(GetPosition(), bullet->GetPosition())> 2048 || bullet->GetDespawn()){
            RemoveBullet(bullet);
            delete (bullet);
            // FIXME: this may crash if you have to delete multiple bullets in a single frame
            // but also i don't see deleting multiple bullets in a frame possible unless using TAS tools
        }
    }
}

void Player::draw() const {
    // DrawRectangle(
    //     static_cast<int>(position.x),
    //     static_cast<int>(position.y),
    //     static_cast<int>(stats.GetSize()),
    //     static_cast<int>(stats.GetSize()),
    //     color
    // );
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

    Vector2 relative_mouse_position = Vector2({
        static_cast<float>(GetMouseX()-GetScreenWidth()/2.0),
        static_cast<float>(GetMouseY()-GetScreenHeight()/2.0),
    });

    Vector2 look_direction = Vector2Multiply(Vector2Normalize(relative_mouse_position), Vector2({64,64}));
    // Draw aim
    DrawRectangle(
        look_direction.x+GetCenter().x-16,
        look_direction.y+GetCenter().y-16,
        32,
        32,
        WHITE
    );

    for (auto* bullet : bullets) {
        bullet->draw();
    }
}

Vector2 Player::GetPosition() const {
    return position;
}

Vector2 Player::GetCenter() const {
    return { position.x + stats.GetSize() / 2.0f, position.y + stats.GetSize() / 2.0f };
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
    Vector2 velocity = Vector2Multiply(Vector2Normalize(direction), {300,300});
    PlayerBullet* bullet = new PlayerBullet(
        game_state,
        GetCenter().x, GetCenter().y,
        velocity.x, velocity.y,
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