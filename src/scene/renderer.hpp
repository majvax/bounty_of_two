#pragma once
#include <raylib-cpp.hpp>
#include <memory>
#include "../imgui/menu.hpp"

namespace ImGui { class Context; }
class GameState;


class Renderer
{
    raylib::Texture2D background;
    raylib::Camera2D camera;
    std::vector<std::unique_ptr<MenuBase>> menus;

public:
    Renderer(int width, int height);

    void add_menu(std::unique_ptr<MenuBase> menu);
    void remove_menu(MenuBase* menu);

    void update(ImGui::Context& ctx) const;
	void draw(ImGui::Context& ctx, const GameState& game_state);
};
