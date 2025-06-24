#include "upgrade.hpp"
#include <stdexcept>
#include "../entity/player.hpp"

std::unique_ptr<Upgrade> get_upgrade(std::mt19937& gen) {
    static std::uniform_int_distribution<int> dis(0, 1);

    int index = dis(gen);
    switch (index) {
        case 0:
            return std::make_unique<SizeUpgrade>(gen);
        case 1:
            return std::make_unique<SpeedUpgrade>(gen);
        default:
            throw std::out_of_range("Invalid upgrade index");
    }
}

std::vector<std::unique_ptr<Upgrade>> get_upgrades() {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::vector<std::unique_ptr<Upgrade>> upgrades;
    for (int i = 0; i < 3; ++i)
        upgrades.push_back(get_upgrade(gen));
    return upgrades;
}

void SizeUpgrade::execute(GameState& game_state) {
    for (auto& player : game_state.GetPlayers()) {
        player->GetStats().SetSize(player->GetStats().GetSize() + size_increase);
    }
}

void SpeedUpgrade::execute(GameState& game_state) {
    for (auto& player : game_state.GetPlayers()) {
        player->GetStats().SetSpeed(player->GetStats().GetSpeed() + speed_increase);
    }
}


