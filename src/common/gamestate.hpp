#pragma once
#include <vector>
#include "entities.hpp"

struct gamestate_t
{
    std::vector<entity_t> entities;
    void update(float deltaTime);
};
