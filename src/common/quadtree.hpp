#pragma once

#include "entities.hpp"
#include "visitor.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>


constexpr int DEFAULT_CAPACITY = 8;
constexpr float QUADTREE_DIVISOR = 2.F;


class Quadtree
{
public:
    explicit Quadtree(const sf::FloatRect& bounds, int capacity = DEFAULT_CAPACITY)
      : bounds_(bounds), capacity_(capacity)
    {}

    bool insert(entity_t* entity)
    {
        if (bounds_.findIntersection(getEntityBounds(*entity)) == std::nullopt) { return false; }

        if (children_.empty() && objects_.size() < static_cast<std::size_t>(capacity_)) {
            objects_.push_back(entity);
            return true;
        }

        if (children_.empty()) { subdivide(); }

        for (auto& child : children_) {
            if (child->insert(entity)) { return true; }
        }
        return false;
    }

    void query(const sf::FloatRect& range, std::vector<entity_t*>& result) const
    {
        if (!bounds_.findIntersection(range)) { return; }

        for (auto* obj : objects_) {
            if (range.findIntersection(getEntityBounds(*obj))) { result.push_back(obj); }
        }

        if (!children_.empty()) {
            for (const auto& child : children_) { child->query(range, result); }
        }
    }

    // Helper to get bounds from entity_t
    static sf::FloatRect getEntityBounds(const entity_t& entity)
    {
        static constexpr auto get_bounds_visitor =
          make_visitor([](const auto& ent) -> sf::FloatRect { return ent.get_bounds(); });
        return visit_ctx(get_bounds_visitor, entity);
    }
    void subdivide()
    {
        float halfWidth = bounds_.size.x / QUADTREE_DIVISOR;
        float halfHeight = bounds_.size.y / QUADTREE_DIVISOR;
        float x = bounds_.position.x;
        float y = bounds_.position.y;

        children_.emplace_back(
          std::make_unique<Quadtree>(sf::FloatRect({ x, y }, { halfWidth, halfHeight }), capacity_));
        children_.emplace_back(
          std::make_unique<Quadtree>(sf::FloatRect({ x + halfWidth, y }, { halfWidth, halfHeight }), capacity_));
        children_.emplace_back(
          std::make_unique<Quadtree>(sf::FloatRect({ x, y + halfHeight }, { halfWidth, halfHeight }), capacity_));
        children_.emplace_back(std::make_unique<Quadtree>(
          sf::FloatRect({ x + halfWidth, y + halfHeight }, { halfWidth, halfHeight }), capacity_));

        // Redistribute existing objects to children
        for (auto* obj : objects_) {
            for (auto& child : children_) {
                if (child->insert(obj)) { break; }
            }
        }
        objects_.clear();
    }


    sf::FloatRect bounds_;
    int capacity_;
    std::vector<entity_t*> objects_;
    std::vector<std::unique_ptr<Quadtree>> children_;
};
