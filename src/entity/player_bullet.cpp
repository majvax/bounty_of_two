#include "player_bullet.hpp"
#include <raylib.h>
#include <raymath.h>

PlayerBullet::PlayerBullet(float x, float y, float vx, float vy, float hitbox_radius, int damage)
	: position({x,y}), velocity({vx,vy}), hitbox_radius(hitbox_radius), damage(damage)
{}

void PlayerBullet::update(float deltaTime){
	position = Vector2({
		position.x + velocity.x * deltaTime,
		position.y+velocity.y*deltaTime
	});
}

void PlayerBullet::draw() const{
	DrawCircle(position.x, position.y, hitbox_radius, BLACK);
}