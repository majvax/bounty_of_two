#pragma once
#include "menu.hpp"
#include <imgui.h>
#include "../helper/assert.hpp"

constexpr unsigned int NOEDIT_FLAGS = ImGuiWindowFlags_NoCollapse
| ImGuiWindowFlags_NoSavedSettings
| ImGuiWindowFlags_NoMove
| ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoTitleBar;



class FPSCounter : public MenuBase
{
public:
    void render(ImGui::Context& ctx) const override
    {
        ASSERT_MISSING_IMGUI_CTX_LOCK(ctx);
        if (!ctx.getWindow()) return;

		ImGui::SetNextWindowSize(ImVec2(100, 35), ImGuiCond_Once);
		ImGui::Begin("FPS Counter", nullptr, NOEDIT_FLAGS);
		ImGui::Text("FPS: %d", ctx.getWindow()->GetFPS());
		ImGui::End();
    }

    void reset() override {}
};
