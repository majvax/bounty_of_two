#include "scene.hpp"

std::vector<std::shared_ptr<Scene>> Scene::current_scenes{};

void Scene::update(float deltatime, ImGui::Context& ctx ) {
    game_state.update(deltatime);
    renderer.update(ctx);
}

void Scene::draw(ImGui::Context& ctx) {
    renderer.draw(ctx, game_state);
}


const decltype(Scene::current_scenes) Scene::GetCurrentScenes() {
    return current_scenes;
}

void Scene::AddScene(std::shared_ptr<Scene> scene) {
    current_scenes.push_back(std::move(scene));
}

void Scene::SetScene(std::shared_ptr<Scene> scene) {
    current_scenes.clear();
    current_scenes.push_back(std::move(scene));
}

void Scene::PopScene() {
    if (!current_scenes.empty()) {
        current_scenes.pop_back();
    }
}
