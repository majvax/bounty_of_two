#include "entities.hpp"
#include "gamestate.hpp"
#include "quadtree.hpp"
#include "visitor.hpp"
#include "visit.hpp"


void GameState::update(float deltaTime)
{
    for (auto& entity : entities) { visit_ctx(update_visitor, entity, deltaTime); }

    sf::FloatRect worldBounds({ 0.f, 0.f }, { 1920.f, 1080.f });
    Quadtree quadtree(worldBounds);

    std::vector<entity_t*> entityPtrs;
    entityPtrs.reserve(entities.size());
    for (auto& ent : entities) { entityPtrs.push_back(&ent); }
    for (auto* ptr : entityPtrs) { quadtree.insert(ptr); }

    for (auto* entPtr : entityPtrs) {
        sf::FloatRect entBounds = Quadtree::getEntityBounds(*entPtr);

        std::vector<entity_t*> potentialColliders;
        quadtree.query(entBounds, potentialColliders);

        for (auto* other : potentialColliders) {
            if (other == entPtr) {
                continue;// Skip self
            }

            sf::FloatRect otherBounds = quadtree.getEntityBounds(*other);
            if (entBounds.findIntersection(otherBounds)) {
                visit_ctx(make_visitor( [](auto& ent) { ent.add_flags(EntityFlags::Damaged); } ), *other);
            }
        }
    }
}
