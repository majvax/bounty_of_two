#include "scene.hpp"
#include <ranges>
#include <algorithm>
#include "../imgui/imgui.hpp"
#include "../imgui/menu.hpp"

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

void Scene::add_entity(EntityBase* entity) {
	entities.push_back(entity);
}

void Scene::remove_entity(EntityBase* entity) {
	auto it = std::ranges::find(entities, entity);
    if (it != entities.end()) {
        entities.erase(it);
    }
}

void Scene::add_menu(MenuBase* menu) {
    menus.push_back(menu);
}

void Scene::remove_menu(MenuBase* menu) {
    auto it = std::ranges::find(menus, menu);
    if (it != menus.end()) {
        menus.erase(it);
    }
}
