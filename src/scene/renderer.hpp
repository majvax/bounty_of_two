#pragma once
#include <raylib-cpp.hpp>
#include <memory>
#include "../imgui/menu.hpp"
#include "../utils/getter_setter.hpp"

namespace ImGui { class Context; }
class GameState;


class Renderer
{
    raylib::Texture2D background;
    raylib::Camera2D camera;
    std::vector<std::unique_ptr<MenuBase>> menus;
    std::vector<raylib::Texture2D> background_elements;
    std::vector<raylib::Vector3> bgelem_positions;

protected:
    int width;
    int height;

public:
    Renderer(int width, int height);

    void add_menu(std::unique_ptr<MenuBase> menu);
    void remove_menu(MenuBase* menu);

    void update(ImGui::Context& ctx) const;
	void draw(ImGui::Context& ctx, const GameState& game_state);


    GETTER_SETTER(int, Width, width)
    GETTER_SETTER(int, Height, height)
};
