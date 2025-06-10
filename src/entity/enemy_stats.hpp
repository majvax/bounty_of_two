#pragma once
#include "../utils/getter_setter.hpp"

class EnemyStats
{
private:
    float attack_speed;
    float speed;
    float health;

public:
    GETTER_SETTER(float, Health, health)
    GETTER_SETTER(float, AtqSpeed, attack_speed)
    GETTER_SETTER(float, Speed, speed)
};
