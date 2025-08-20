#pragma once
#include <vector>
#include "entities.hpp"

struct GameState
{
    std::vector<entity_t> entities;
    void update(float deltaTime);
};
