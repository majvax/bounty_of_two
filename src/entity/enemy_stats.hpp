#pragma once
#include "../utils/getter_setter.hpp"

class EnemyStats
{
private:
    float attack_speed;
    float speed;
    float health;
    float size;

public:
    explicit EnemyStats(float attack_speed = 1.0f, float speed = 200.0f, float health = 30.0f, float size = 40.0f)
        :attack_speed(attack_speed), speed(speed), health(health), size(size){}
    GETTER_SETTER(float, Health, health)
    GETTER_SETTER(float, AtqSpeed, attack_speed)
    GETTER_SETTER(float, Speed, speed)
    GETTER_SETTER(float, Size, size)
};
