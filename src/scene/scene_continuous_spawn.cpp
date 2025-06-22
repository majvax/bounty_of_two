#include "scene_continuous_spawn.hpp"
#include "../entity/enemy_slayer.hpp"
#include "scene.hpp"
#include <iostream>

SceneContinuousSpawn::SceneContinuousSpawn(int width, int height, int max_enemies_on_screen)
    : Scene(width, height), window_width(width), window_height(height), renderer(width, height),
    max_enemies_on_screen(max_enemies_on_screen) {}


void SceneContinuousSpawn::update(float deltatime, ImGui::Context& ctx ) {
    //std::cout << "bonjour" << std::endl;
    if (game_state.GetEntities().size() < max_enemies_on_screen){
        auto slayer = std::make_unique<EnemySlayer>(&game_state, window_width, window_height);
        slayer->SetTarget(game_state.GetPlayers().front().get());
        // move the slayer ownership to the scene
        game_state.add_entity(std::move(slayer));
    }
    game_state.update(deltatime);
    renderer.update(ctx);

    for (size_t i = 0; i < game_state.GetEntities().size(); i++) {
        EnemySlayer* entity_enemy = reinterpret_cast<EnemySlayer*>(game_state.GetEntities()[i].get());
        if (entity_enemy != nullptr){
            if (entity_enemy->IsDead()){
                game_state.remove_entity(entity_enemy);
                i--;
            }
        }
    }
}

void SceneContinuousSpawn::draw(ImGui::Context& ctx) {
    renderer.draw(ctx, game_state);
}
