#pragma once
#include "menu.hpp"
#include <imgui.h>
#include <numeric>
#include "../utils/assert.hpp"

constexpr unsigned int NOEDIT_FLAGS = ImGuiWindowFlags_NoCollapse
| ImGuiWindowFlags_NoSavedSettings
| ImGuiWindowFlags_NoMove
| ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoTitleBar;



class FPSCounter : public MenuBase
{
private:
    static constexpr size_t HISTORY_SIZE = 1024;
    float fpsHistory[HISTORY_SIZE] = {};
    size_t historyIndex = 0;
    size_t historyCount = 0;
public:
    void update(ImGui::Context& ctx) override
    {
        if (!ctx.getWindow()) return;
        
        float fps = static_cast<float>(ctx.getWindow()->GetFPS());
        fpsHistory[historyIndex] = fps;
        historyIndex = (historyIndex + 1) % HISTORY_SIZE;
        if (historyCount < HISTORY_SIZE) ++historyCount;
    }

    void render(ImGui::Context& ctx) const override
    {
        ASSERT_MISSING_IMGUI_CTX_LOCK(ctx);
        if (!ctx.getWindow()) return;

        float avgFps = 0.0f;
        if (historyCount > 0) {
            float sum = 0.0f;
            for (size_t i = 0; i < historyCount; ++i)
                sum += fpsHistory[i];
            avgFps = sum / static_cast<float>(historyCount);
        }

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(100, 35), ImGuiCond_Once);
		ImGui::Begin("FPS Counter", nullptr, NOEDIT_FLAGS);
		ImGui::Text("FPS: %3d", ctx.getWindow()->GetFPS());
        ImGui::Text("Avg: %.1f", avgFps);
		ImGui::End();
    }

    void reset() override {}
};
