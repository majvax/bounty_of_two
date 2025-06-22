#include "base.hpp"
#include <raylib.h>

class PlayerBullet : public EntityBase {
private:
	Vector2 position;
	Vector2 velocity;
	float hitbox_radius;
	int damage;
public:
	~PlayerBullet() = default;
	PlayerBullet(float x, float y, float vx, float vy, float hitbox_radius, int damage);
	void update(float deltatime) override;
	void draw() const override;
	Vector2 GetPosition() const { return position; }
};