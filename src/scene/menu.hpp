#pragma once
#include "scene.hpp"

// Scene for the main menu (before starting a game)
class MenuScene : public Scene {
    raylib::Texture2D title_texture;
public:
    MenuScene(int width, int height);
    void update(float deltatime, ImGui::Context& ctx) override;
    void draw(ImGui::Context& ctx) override;
};
