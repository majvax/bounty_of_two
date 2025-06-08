#pragma once
#include "menu.hpp"
#include <imgui.h>
#include "../helper/assert.hpp"

constexpr unsigned int FLAGS = ImGuiWindowFlags_NoSavedSettings
| ImGuiWindowFlags_NoResize;



class PlayerMenu : public MenuBase
{
public:
    void render(ImGui::Context& ctx) const override
    {
        ASSERT_MISSING_IMGUI_CTX_LOCK(ctx);
        if (!ctx.getWindow()) return;

        auto player = ctx.getPlayer();

		ImGui::Begin("Player Stat", nullptr, FLAGS);
        ImGui::Text("Position: (%.2f, %.2f)", player->GetPosition().x, player->GetPosition().y);
        ImGui::Text("Speed: %.2f", player->GetSpeed());
        ImGui::Text("Health: %d", player->GetHealth());
        ImGui::Text("Crit Chance: %.2f%%", player->GetCritChance() * 100.0f);
        ImGui::Text("Crit Multiplier: %.2f", player->GetCritMultiplier());
		ImGui::End();
    }

    void reset() override {}
};
