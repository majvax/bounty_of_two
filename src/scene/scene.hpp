#pragma once
#include "../imgui/imgui.hpp"
#include "../utils/getter_setter.hpp"
#include "game_state.hpp"
#include "renderer.hpp"

#include <memory>
#include <vector>

class Scene
{

    static std::vector<std::shared_ptr<Scene>> current_scenes;
protected:
    Renderer renderer;
    GameState& game_state;
public:
    virtual void update(float deltatime, ImGui::Context& ctx);
    virtual void draw(ImGui::Context& ctx);
    Scene(int width, int height) : renderer(width, height), game_state(GameState::GetInstance()) {}
    

    Renderer& GetRenderer() { return renderer; }
    GameState& GetGameState() { return game_state; }
    static void AddScene(std::shared_ptr<Scene> scene);
    static void SetScene(std::shared_ptr<Scene> scene);
    static void PopScene();
    static const decltype(current_scenes) GetCurrentScenes();
};
