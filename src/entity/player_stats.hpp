#pragma once
#include "../utils/getter_setter.hpp"

class PlayerStats
{
private:
    int life;
    int damage;
    float attack_speed;
    float speed;
    float acceleration;
    float crit_chance;
    float crit_multiplier;
    float loadingTime;
    int dash;
    int pierce;
    float sizeof_area;
    float size;

public:
    explicit PlayerStats (int life = 3, int damage = 30, float attack_speed = 1.0f, float speed = 250.0f, float crit_chance = 5.0f, float crit_multiplier = 100.0f, float loadingTime = 100.0f, int dash = 1, int pierce = 0, float sizeof_area = 100.0f, float size = 40.0f, float acceleration = 750.0f )
        : life(life), damage(damage), attack_speed(attack_speed), speed(speed), crit_chance(crit_chance), crit_multiplier(crit_multiplier), loadingTime(loadingTime), dash(dash), pierce(pierce), sizeof_area(sizeof_area), size(size), acceleration(acceleration)
            {}
    GETTER_SETTER(int, Life, life)
    GETTER_SETTER(int, Damage, damage)
    GETTER_SETTER(float, AtqSpeed, attack_speed)
    GETTER_SETTER(float, Speed, speed)
    GETTER_SETTER(float, CritChance, crit_chance)
    GETTER_SETTER(float, CritMultiplier, crit_multiplier)
    GETTER_SETTER(float, LoadingTime, loadingTime)
    GETTER_SETTER(int, Dash, dash)
    GETTER_SETTER(int, Pierce, pierce)
    GETTER_SETTER(float, SizeofArea, sizeof_area)
    GETTER_SETTER(float, Size, size)
    GETTER_SETTER(float, Acceleration, acceleration)

};
