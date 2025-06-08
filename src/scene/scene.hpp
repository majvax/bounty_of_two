#pragma once

#include "../entity/base.hpp"
#include "../entity/player.hpp"
#include <vector>
#include <memory>

class MenuBase;
namespace ImGui { class Context; }
class Scene {
private:
	std::vector<std::unique_ptr<EntityBase>> entities;
    std::vector<std::unique_ptr<MenuBase>> menus;
public:
	Scene();
    void add_entity(std::unique_ptr<EntityBase> entity);
	void remove_entity(EntityBase* entity);

    void add_menu(std::unique_ptr<MenuBase> menu);
    void remove_menu(MenuBase* menu);

	void update(float deltaTime);
	void draw(ImGui::Context& ctx) const;

    Player* get_player() const;
};
