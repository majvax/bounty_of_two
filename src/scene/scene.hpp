#pragma once

#include <raylib-cpp.hpp>
#include "../entity/base.hpp"
#include "../entity/player.hpp"
#include <vector>
#include <memory>

class MenuBase;
namespace ImGui { class Context; }
class Scene {
private:
    raylib::Texture2D background;
    raylib::Camera2D camera;
	std::vector<std::unique_ptr<EntityBase>> entities;
    std::vector<std::unique_ptr<MenuBase>> menus;
public:
	Scene(int width, int height);
    void add_entity(std::unique_ptr<EntityBase> entity);
	void remove_entity(EntityBase* entity);

    void add_menu(std::unique_ptr<MenuBase> menu);
    void remove_menu(MenuBase* menu);

	void update(float deltaTime);
	void draw(ImGui::Context& ctx);

    Player* get_player() const;
};
