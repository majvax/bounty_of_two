#include "visitor.hpp"
#include "entities.hpp"


constexpr void reset(auto& ent)
{
    if (ent.position.x < 0 || ent.position.y < 0 || ent.position.x > 1920 || ent.position.y > 1080) {
        // Reset position if out of bounds
        // make them bounce off the walls
        if (ent.position.x < 0) ent.position.x = 0;
        if (ent.position.y < 0) ent.position.y = 0;
        if (ent.position.x > 1920) ent.position.x = 1920;
        if (ent.position.y > 1080) ent.position.y = 1080;
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
