
#include "scene.hpp"

Scene::Scene() {}

void Scene::update(float deltaTime) {
	for (int eid = 0; eid < entities.size(); eid++) {
		entities[eid]->process(deltaTime);
	}
}

void Scene::draw() {
	for (int eid = 0; eid < entities.size(); eid++) {
		entities[eid]->draw();
	}
}

void Scene::add_entity(EntityBase* entity) {
	entities.push_back(entity);
}

void Scene::remove_entity(EntityBase* entity) {
	for (int eid = 0; eid < entities.size(); eid++) {
		if (entities[eid] == entity){
			entities.erase(std::next(entities.begin(), eid));
			return; // only delete one entity at a time here
		};
	}
}