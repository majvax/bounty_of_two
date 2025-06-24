
#pragma once
#include "menu.hpp"
#include <imgui.h>
#include "../utils/assert.hpp"
#include "../scene/scene.hpp"
#include "../scene/menu.hpp"


class EscMenu : public MenuBase
{
public:
    void update(ImGui::Context& ctx) override
    {
    }

    void render(ImGui::Context& ctx) const override
    {
            ASSERT_MISSING_IMGUI_CTX_LOCK(ctx);
    
    // Center the menu window

    }


    void reset() override {}
};
