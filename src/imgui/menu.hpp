#pragma once
#include "imgui.hpp"

class Joueur;

class MenuBase
{
public:
    virtual ~MenuBase() = default;
    virtual void render(ImGui::Context& ctx) const = 0;
    virtual void reset() = 0;
};
