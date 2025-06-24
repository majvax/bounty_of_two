#pragma once
#include "scene.hpp"
#include "../upgrade/upgrade.hpp"

class UpgradeScene : public Scene {
private:
    std::vector<std::unique_ptr<Upgrade>> upgrades;
public:
    UpgradeScene(int width, int height);
    void update(float deltatime, ImGui::Context& ctx) override;
    void draw(ImGui::Context& ctx) override;
};
