#pragma once
#include "menu.hpp"
#include <imgui.h>
#include "../utils/assert.hpp"

constexpr unsigned int NOEDIT_FLAGS = ImGuiWindowFlags_NoCollapse
| ImGuiWindowFlags_NoSavedSettings
| ImGuiWindowFlags_NoMove
| ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoTitleBar;



class FPSCounter : public MenuBase
{
private:
public:
    void update(ImGui::Context& ctx) override
    {
        ASSERT_MISSING_IMGUI_CTX_LOCK(ctx);

    }

    void render(ImGui::Context& ctx) const override
    {
        ASSERT_MISSING_IMGUI_CTX_LOCK(ctx);
        if (!ctx.getWindow()) return;

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(100, 35), ImGuiCond_Once);
		ImGui::Begin("FPS Counter", nullptr, NOEDIT_FLAGS);
		ImGui::Text("FPS: %3d", ctx.getWindow()->GetFPS());
		ImGui::End();
    }

    void reset() override {}
};
