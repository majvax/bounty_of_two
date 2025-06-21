#include "player.hpp"
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <raymath.h>
#include <string>

Player::Player(float x, float y, Color color)
    : position({ x, y }), color(color), current_sprite(0),
    direction(DOWN), frame_timer(0), flip_h(false) {
    for (size_t i = 0; i < 7; i++) {
        Texture2D image = LoadTexture(("assets/crocoimages/image000" + std::to_string(i) + ".png").c_str());
        sprites.push_back(image);
    }
}

void Player::update(float deltaTime) {
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
}

void Player::draw() const {
    DrawRectangle(
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(stats.GetSize()),
        static_cast<int>(stats.GetSize()),
        color
    );
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
