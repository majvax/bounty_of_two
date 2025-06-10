#pragma once
#include "../utils/getter_setter.hpp"

class PlayerStats
{
private:
    int life;
    int damage;
    float attack_speed;
    float speed;
    float crit_chance;
    float crit_multiplier;
    float loadingTime;
    int dash;
    int pierce;
    float sizeof_area;
    float size;

public:
    GETTER_SETTER(int, Life, life)
    GETTER_SETTER(int, Damage, damage)
    GETTER_SETTER(int, Attack_speed, attack_speed)
    GETTER_SETTER(float, Speed, speed)
    GETTER_SETTER(float, CritChance, crit_chance)
    GETTER_SETTER(float, CritMultiplier, crit_multiplier)
    GETTER_SETTER(float, LoadingTime, loadingTime)
    GETTER_SETTER(float, Dash, dash)
    GETTER_SETTER(float, Pierce, pierce)
    GETTER_SETTER(float, Sizeof_area, sizeof_area)
    GETTER_SETTER(float, Size, size)

};
