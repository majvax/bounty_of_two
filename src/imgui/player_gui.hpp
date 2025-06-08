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
        
        ImGui::PushID(10);
        ImGui::Text("Speed: %.2f", player->GetSpeed());
        #ifdef DEBUG
        ImGui::SameLine();
        if (ImGui::Button("-1##"))
            player->SetSpeed(player->GetSpeed() - 1.0f);
        ImGui::SameLine();
        if (ImGui::Button("+1##"))
            player->SetSpeed(player->GetSpeed() + 1.0f);
        #endif
        ImGui::PopID();

        ImGui::PushID(20);
        ImGui::Text("Health: %d", player->GetHealth());
        #ifdef DEBUG
        ImGui::SameLine();
        if (ImGui::Button("-1##"))
            player->TakeDamage(1);
        ImGui::SameLine();
        if (ImGui::Button("+1##"))
            player->SetHealth(player->GetHealth() + 1);
        #endif
        ImGui::PopID();

        ImGui::PushID(30);
        ImGui::Text("Size: %.2f", player->GetSize());
        #ifdef DEBUG
        ImGui::SameLine();
        if (ImGui::Button("-1##"))
            player->SetSize(player->GetSize() - 1.0f);
        ImGui::SameLine();
        if (ImGui::Button("+1##"))
            player->SetSize(player->GetSize() + 1.0f);
        #endif
        ImGui::PopID();

        ImGui::PushID(40);
        ImGui::Text("Crit Chance: %.2f%%", player->GetCritChance() * 100.0f);
        #ifdef DEBUG
        ImGui::SameLine();
        if (ImGui::Button("-1"))
            player->SetCritChance(player->GetCritChance() - 0.01f);
        ImGui::SameLine();
        if (ImGui::Button("+1"))
            player->SetCritChance(player->GetCritChance() + 0.01f);
        #endif
        ImGui::PopID();

        ImGui::PushID(50);
        ImGui::Text("Crit Multiplier: %.2f", player->GetCritMultiplier());
        #ifdef DEBUG
        ImGui::SameLine();
        if (ImGui::Button("-0.1"))
            player->SetCritMultiplier(player->GetCritMultiplier() - 0.1f);
        ImGui::SameLine();
        if (ImGui::Button("+0.1"))
            player->SetCritMultiplier(player->GetCritMultiplier() + 0.1f);
        #endif
        ImGui::PopID();

		ImGui::End();
    }

    void reset() override {}
};
