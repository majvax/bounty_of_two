#include "entities.hpp"
#include "gamestate.hpp"
#include "quadtree.hpp"
#include "visitor.hpp"


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

// Same visitor for example purpose
constexpr auto update_visitor = make_unique_visitor<entity_t, float>(
  [](sniper_t& ent, float deltaTime) {
      if (ent.has_flags(EntityFlags::Damaged)) { ent.clear_flags(EntityFlags::Damaged); }
      ent.position += ent.velocity * deltaTime;
      reset(ent);
  },
  [](berserker_t& ent, float deltaTime) {
      if (ent.has_flags(EntityFlags::Damaged)) { ent.clear_flags(EntityFlags::Damaged); }
      ent.position += ent.velocity * deltaTime;
      reset(ent);
  },
  [](player_t& ent, float deltaTime) {
      if (ent.has_flags(EntityFlags::Damaged)) { ent.clear_flags(EntityFlags::Damaged); }
      ent.position += ent.velocity * deltaTime;
      reset(ent);
  });


void GameState::update(float deltaTime)
{
    // First, update positions (your existing code)
    for (auto& entity : entities) { visit_ctx(update_visitor, entity, deltaTime); }

    // Build quadtree for collisions
    sf::FloatRect worldBounds({ 0.f, 0.f }, { 1920.f, 1080.f });// From your hardcoded values
    Quadtree quadtree(worldBounds);

    // Insert all entities (store raw pointers—fine since entities live in the vector)
    std::vector<entity_t*> entityPtrs;
    entityPtrs.reserve(entities.size());
    for (auto& ent : entities) { entityPtrs.push_back(&ent); }
    for (auto* ptr : entityPtrs) { quadtree.insert(ptr); }

    // Check collisions for each entity
    for (auto* entPtr : entityPtrs) {
        sf::FloatRect entBounds = Quadtree::getEntityBounds(*entPtr);// Reuse the helper

        std::vector<entity_t*> potentialColliders;
        quadtree.query(entBounds, potentialColliders);

        // Narrow-phase: check actual overlaps and respond
        for (auto* other : potentialColliders) {
            if (other == entPtr) {
                continue;// Skip self
            }

            sf::FloatRect otherBounds = quadtree.getEntityBounds(*other);
            if (entBounds.findIntersection(otherBounds)) {
                std::visit(Visitor{ [](auto& ent) { ent.add_flags(EntityFlags::Damaged); } }, *other);
            }
        }
    }
}
