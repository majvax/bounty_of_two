#include "scene.hpp"
#include <ranges>


Scene::Scene() {}

void Scene::update(float deltaTime) {
	for (auto& entity : entities) {
        entity->update(deltaTime);
    }
}

void Scene::draw() const {
	for (const auto& entity : entities) {
        entity->draw();
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
