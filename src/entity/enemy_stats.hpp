#pragma once
#include "../utils/getter_setter.hpp"

class EnemyStats
{
private:
    float attack_speed;
    float speed;
    int health;
    float size;

public:
    EnemyStats(float attack_speed = 1.0f, float speed = 200.0f, int health = 30, float size = 40.0f)
        :attack_speed(attack_speed), speed(speed), health(health), size(size){}
    GETTER_SETTER(float, Health, health)
    GETTER_SETTER(float, AtqSpeed, attack_speed)
    GETTER_SETTER(float, Speed, speed)
    GETTER_SETTER(float, Size, size)
};
