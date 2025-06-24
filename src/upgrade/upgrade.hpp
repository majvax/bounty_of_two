#pragma once
#include <random>
#include <memory>
#include <vector>
#include "../scene/game_state.hpp"
#include <string>


enum UpgradeType {
    SIZE_UPGRADE,
    SPEED_UPGRADE,
};

namespace std
{
    inline string to_string(UpgradeType type) {
        switch (type) {
            case UpgradeType::SIZE_UPGRADE: return "Size Upgrade";
            case UpgradeType::SPEED_UPGRADE: return "Speed Upgrade";
            default: return "Unknown Upgrade";
        }
    }
}



class Upgrade
{
UpgradeType type;

public:
    Upgrade(UpgradeType type) : type(type) {}
    virtual ~Upgrade() = default;
    virtual void execute(GameState& game_state) = 0;
    UpgradeType get_type() const { return type; }
};


class SizeUpgrade : public Upgrade
{
private:
    int size_increase = 0;
public:
    SizeUpgrade(std::mt19937& gen) : Upgrade(UpgradeType::SIZE_UPGRADE) {
        std::uniform_int_distribution<int> dis(1, 5);
        size_increase = dis(gen);
    }
    void execute(GameState& game_state) override;
};


class SpeedUpgrade : public Upgrade
{
private:
    int speed_increase = 0;
public:
    SpeedUpgrade(std::mt19937& gen) : Upgrade(UpgradeType::SPEED_UPGRADE) {
        std::uniform_int_distribution<int> dis(1, 5);
        speed_increase = dis(gen);
    }
    void execute(GameState& game_state) override;
};


std::vector<std::unique_ptr<Upgrade>> get_upgrades();






