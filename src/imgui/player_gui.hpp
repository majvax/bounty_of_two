
#pragma once
#include "menu.hpp"
#include <imgui.h>
#include "../utils/assert.hpp"

constexpr unsigned int FLAGS = ImGuiWindowFlags_NoSavedSettings
| ImGuiWindowFlags_NoResize;


/**
 * @class PlayerMenu
 * @brief A basic implementation of a player statistics menu using ImGui.
 *
 * This class derives from MenuBase and provides a simple GUI for displaying and (in debug mode)
 * modifying player attributes such as position, speed, health, size, critical chance, and critical multiplier.
 * The menu is rendered using ImGui and is intended for debugging and development purposes.
 *
 * @note This is a basic implementation for now and may be extended with additional features in the future.
 */
class PlayerMenu : public MenuBase
{
public:
    void update(ImGui::Context& ctx) override
    {
    }

    void render(ImGui::Context& ctx) const override
    {
        ASSERT_MISSING_IMGUI_CTX_LOCK(ctx);
        if (!ctx.getWindow()) return;

        auto player = ctx.getPlayer();

		ImGui::Begin("Player Stat", nullptr, FLAGS);
        ImGui::Text("Position: (%.2f, %.2f)", player->GetPosition().x, player->GetPosition().y);
        
        ImGui::PushID(10);
        ImGui::Text("Speed: %.2f", player->GetStats().GetSpeed());
        #ifdef DEBUG
        ImGui::SameLine();
        if (ImGui::Button("-1##"))
            player->GetStats().SetSpeed(player->GetStats().GetSpeed() - 1.0f);
        ImGui::SameLine();
        if (ImGui::Button("+1##"))
            player->GetStats().SetSpeed(player->GetStats().GetSpeed() + 1.0f);
        #endif
        ImGui::PopID();

        ImGui::PushID(20);
        ImGui::Text("Health: %d", player->GetStats().GetLife());
        #ifdef DEBUG
        ImGui::SameLine();
        if (ImGui::Button("-1##"))
            player->TakeDamage();
        ImGui::SameLine();
        if (ImGui::Button("+1##"))
            player->GetStats().SetLife(player->GetStats().GetLife() + 1);
        #endif
        ImGui::PopID();

        ImGui::PushID(30);
        ImGui::Text("Size: %.2f", player->GetStats().GetSize());
        #ifdef DEBUG
        ImGui::SameLine();
        if (ImGui::Button("-1##"))
            player->GetStats().SetSize(player->GetStats().GetSize() - 1.0f);
        ImGui::SameLine();
        if (ImGui::Button("+1##"))
            player->GetStats().SetSize(player->GetStats().GetSize() + 1.0f);
        #endif
        ImGui::PopID();

        ImGui::PushID(40);
        ImGui::Text("Crit Chance: %.2f%%", player->GetStats().GetCritChance());
        #ifdef DEBUG
        ImGui::SameLine();
        if (ImGui::Button("-1"))
            player->GetStats().SetCritChance(player->GetStats().GetCritChance() - 0.01f);
        ImGui::SameLine();
        if (ImGui::Button("+1"))
            player->GetStats().SetCritChance(player->GetStats().GetCritChance() + 0.01f);
        #endif
        ImGui::PopID();

        ImGui::PushID(50);
        ImGui::Text("Crit Multiplier: %.2f", player->GetStats().GetCritMultiplier());
        #ifdef DEBUG
        ImGui::SameLine();
        if (ImGui::Button("-0.1"))
            player->GetStats().SetCritMultiplier(player->GetStats().GetCritMultiplier() - 0.1f);
        ImGui::SameLine();
        if (ImGui::Button("+0.1"))
            player->GetStats().SetCritMultiplier(player->GetStats().GetCritMultiplier() + 0.1f);
        #endif
        ImGui::PopID();

		ImGui::End();
    }

    void reset() override {}
};
