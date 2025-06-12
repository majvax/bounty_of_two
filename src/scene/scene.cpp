#include "scene.hpp"


void Scene::update(float deltatime, ImGui::Context& ctx ) {
    game_state.update(deltatime);
    renderer.update(ctx);
}

void Scene::draw(ImGui::Context& ctx) {
    renderer.draw(ctx, game_state);
}
