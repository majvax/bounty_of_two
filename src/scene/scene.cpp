#include "scene.hpp"

std::unique_ptr<Scene> Scene::current_scene = nullptr;

void Scene::update(float deltatime, ImGui::Context& ctx ) {
    game_state.update(deltatime);
    renderer.update(ctx);
}

void Scene::draw(ImGui::Context& ctx) {
    renderer.draw(ctx, game_state);
}


void Scene::SetCurrentScene(std::unique_ptr<Scene> scene) {
    current_scene = std::move(scene);
}

Scene* Scene::GetCurrentScene() {
    return current_scene.get();
}
