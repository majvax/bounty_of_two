#pragma once

#include "../entity/player.hpp"
#include <vector>

class MenuBase;
namespace ImGui { class Context; }
class Scene {
private:
    // La scene doit avoir l'ownership des entités et des menus
    // TODO: refactor using std::unique_ptr
    // currently using raw pointers for simplicity
    // It's leaking memory though, so this should be fixed later
	std::vector<EntityBase*> entities;
    std::vector<MenuBase*> menus;
public:
	Scene();
	void add_entity(EntityBase* entity);
	void remove_entity(EntityBase* entity);

    void add_menu(MenuBase* menu);
    void remove_menu(MenuBase* menu);

	void update(float deltaTime);
	void draw(ImGui::Context& ctx) const;
};
