#pragma once
#include "../utils/getter_setter.hpp"

class EnemyStats
{
private:
    int life;
    float attack_speed;
    float speed;
    float health;

public:
    GETTER_SETTER(int, Health, health)
    GETTER_SETTER(int, Attack_speed, attack_speed)
    GETTER_SETTER(float, Speed, speed)
    GETTER_SETTER(float, Size, size)

};