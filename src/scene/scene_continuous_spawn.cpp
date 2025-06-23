#include "scene_continuous_spawn.hpp"
#include "../entity/enemy_slayer.hpp"
#include "scene.hpp"
#include <iostream>

SceneContinuousSpawn::SceneContinuousSpawn(int width, int height, int max_enemies_on_screen)
    : Scene(width, height), max_enemies_on_screen(max_enemies_on_screen), window_width(width), window_height(height), renderer(width, height),
    game_state() {}


void SceneContinuousSpawn::update(float deltatime, ImGui::Context& ctx ) {
    //std::cout << "bonjour" << std::endl;
    if (GetGameState().GetEntities().size() < max_enemies_on_screen){
        auto slayer = std::make_unique<EnemySlayer>(&GetGameState(), window_width, window_height);
        slayer->SetTarget(GetGameState().GetPlayers().front().get());
        // move the slayer ownership to the scene
        GetGameState().add_entity(std::move(slayer));
    }
    GetGameState().update(deltatime);
    GetRenderer().update(ctx);

    for (size_t i = 0; i < GetGameState().GetEntities().size(); i++) {
        EnemySlayer* entity_enemy = reinterpret_cast<EnemySlayer*>(GetGameState().GetEntities()[i].get());
        if (entity_enemy != nullptr){
            if (entity_enemy->IsDead()){
                GetGameState().remove_entity(entity_enemy);
                i--;
            }
        }
    }
}

void SceneContinuousSpawn::draw(ImGui::Context& ctx) {
    GetRenderer().draw(ctx, GetGameState());
}
