#include "scene_continuous_spawn.hpp"
#include "../entity/enemy_slayer.hpp"
#include "scene.hpp"
#include <iostream>
#include <string>

SceneContinuousSpawn::SceneContinuousSpawn(int width, int height, int max_enemies_on_screen)
    : Scene(width, height), max_enemies_on_screen(max_enemies_on_screen), window_width(width), window_height(height), score(0), renderer(width, height),
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

    if (score/25 > max_enemies_on_screen-2) max_enemies_on_screen += 1;

    for (size_t i = 0; i < GetGameState().GetEntities().size(); i++) {
        EnemySlayer* entity_enemy = reinterpret_cast<EnemySlayer*>(GetGameState().GetEntities()[i].get());
        if (entity_enemy != nullptr){
            if (entity_enemy->IsDead()){
                score++;
                GetGameState().remove_entity(entity_enemy);
                i--;
            }
        }
    }

    if (GetGameState().GetPlayers().size() > 0){
        bool someone_still_alive = false;
        for (int i = 0; i < GetGameState().GetPlayers().size(); i++) {
            if (!GetGameState().GetPlayers()[i]->IsDead()){someone_still_alive = true;}
        }
        if (not someone_still_alive){
            //Scene::SetCurrentScene(std::make_unique<MenuScene>(window_width, window_width));
        }
    }
}

void SceneContinuousSpawn::draw(ImGui::Context& ctx) {
    GetRenderer().draw(ctx, GetGameState());

    DrawText(std::to_string(score).c_str(), window_width/2, 14, 64, {0,0,0,128});
    DrawText(std::to_string(score).c_str(), window_width/2, 10, 64, WHITE);
}
