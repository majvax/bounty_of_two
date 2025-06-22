#include "base.hpp"
#include <raylib.h>

class PlayerBullet : public EntityBase {
private:
    GameState* game_state;
    Vector2 position;
    Vector2 velocity;
    float hitbox_radius;
    int damage;
    bool despawn;
public:
    ~PlayerBullet() = default;
    PlayerBullet(GameState* game_state, float x, float y, float vx, float vy, float hitbox_radius, int damage);
    void update(float deltatime) override;
    void draw() const override;
    Vector2 GetPosition() const { return position; }
    void MarkDespawn(bool despawn_state);
    const bool GetDespawn() const;
};