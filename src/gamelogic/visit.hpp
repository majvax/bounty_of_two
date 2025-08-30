#include "entities.hpp"
#include "visitor.hpp"


constexpr int WINDOW_HEIGHT = 1080;
constexpr int WINDOW_WIDTH = 1920;

constexpr void reset(auto& ent)
{
    if (ent.position.x < 0 || ent.position.y < 0 || ent.position.x > WINDOW_WIDTH || ent.position.y > WINDOW_HEIGHT) {
        // Reset position if out of bounds
        // make them bounce off the walls
        if (ent.position.x < 0) { ent.position.x = 0; }
        if (ent.position.y < 0) { ent.position.y = 0; }
        if (ent.position.x > WINDOW_WIDTH) { ent.position.x = WINDOW_WIDTH; }
        if (ent.position.y > WINDOW_HEIGHT) { ent.position.y = WINDOW_HEIGHT; }
        ent.velocity.x = -ent.velocity.x;
        ent.velocity.y = -ent.velocity.y;
    }
}


constexpr auto update_visitor = make_unique_visitor<entity_t, float>(
  [](sniper_t& ent, float deltaTime) {
      if (ent.has_flag(EntityFlags::Damaged)) { ent.clear_flags(EntityFlags::Damaged); }
      ent.position += ent.velocity * deltaTime;
      reset(ent);
  },
  [](berserker_t& ent, float deltaTime) {
      if (ent.has_flag(EntityFlags::Damaged)) { ent.clear_flags(EntityFlags::Damaged); }
      ent.position += ent.velocity * deltaTime;
      reset(ent);
  },
  [](player_t& ent, float deltaTime) {
      if (ent.has_flag(EntityFlags::Damaged)) { ent.clear_flags(EntityFlags::Damaged); }
      ent.position += ent.velocity * deltaTime;
      reset(ent);
  });
