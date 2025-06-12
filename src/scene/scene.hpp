#pragma once
#include "../imgui/imgui.hpp"
#include "../utils/getter_setter.hpp"
#include "game_state.hpp"
#include "renderer.hpp"




class Scene
{
    Renderer renderer;
    GameState game_state;
public:
    void update(float deltatime, ImGui::Context& ctx);
    void draw(ImGui::Context& ctx);
    Scene(int width, int height) : renderer(width, height) {}


    Renderer& GetRenderer() { return renderer; }
    GameState& GetGameState() { return game_state; }
};
