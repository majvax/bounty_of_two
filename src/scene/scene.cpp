#include "scene.hpp"
#include <ranges>
#include <algorithm>
#include "../imgui/imgui.hpp"
#include "../imgui/menu.hpp"
#include "../helper/assert.hpp"

Scene::Scene() {}

void Scene::update(float deltaTime) {
	for (auto& entity : entities) {
        entity->update(deltaTime);
    }
}

void Scene::draw(ImGui::Context& ctx) const {
    // drawing all the entities
	for (const auto& entity : entities) {
        entity->draw();
    }

    // drawing all the menus
    while (ctx.lock()) {
        for (const auto& menu : menus) {
            menu->render(ctx);
        }
    }
}


void Scene::add_entity(std::unique_ptr<EntityBase> entity) {
    // Moving the ownership of the entity into the vector
    ASSERT_PTR(entity, "Entity must not be null");
    entities.push_back(std::move(entity));
}

void Scene::remove_entity(EntityBase* entity) {
    ASSERT_PTR(entity, "Entity must not be null");
    auto it = std::ranges::find_if(entities, [entity](const auto& ptr) {
        return ptr.get() == entity;
    });
    if (it != entities.end()) {
        entities.erase(it);
    }
}


void Scene::add_menu(std::unique_ptr<MenuBase> menu) {
    ASSERT_PTR(menu, "Menu must not be null");
    menus.push_back(std::move(menu));
}

void Scene::remove_menu(MenuBase* menu) {
    ASSERT_PTR(menu, "Menu must not be null");
    auto it = std::ranges::find_if(menus, [menu](const auto& ptr) {
        return ptr.get() == menu;
    });
    if (it != menus.end()) {
        menus.erase(it);
    }
}


Player* Scene::get_player() const {
    // Assuming the first entity is always the player
    if (entities.empty()) return nullptr;
    return static_cast<Player*>(entities.front().get());
}
