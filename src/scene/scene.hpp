#pragma once
#include "../imgui/imgui.hpp"
#include "../utils/getter_setter.hpp"
#include "game_state.hpp"
#include "renderer.hpp"




class Scene
{

    static std::unique_ptr<Scene> current_scene;

    Renderer renderer;
    GameState game_state;
public:
    virtual void update(float deltatime, ImGui::Context& ctx);
    virtual void draw(ImGui::Context& ctx);
    Scene(int width, int height) : renderer(width, height) {}


    Renderer& GetRenderer() { return renderer; }
    GameState& GetGameState() { return game_state; }

    static Scene* GetCurrentScene();
    static void SetCurrentScene(std::unique_ptr<Scene> scene);
};
