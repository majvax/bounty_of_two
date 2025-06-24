#include "scene_continuous_spawn.hpp"
#include "../entity/enemy_slayer.hpp"
#include "scene.hpp"
#include <iostream>
#include <string>
#include <memory>
#include "menu.hpp"
#include "esc_menu.hpp"

SceneContinuousSpawn::SceneContinuousSpawn(int width, int height, int max_enemies_on_screen)
    : Scene(width, height), max_enemies_on_screen(max_enemies_on_screen), window_width(width), window_height(height), renderer(width, height),
    game_state(), score(0), exp(0), exp_pts(0) {}


void SceneContinuousSpawn::update(float deltatime, ImGui::Context& ctx ) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        Scene::AddScene(std::make_shared<EscMenuScene>(window_width, window_height));
        return;
    }


    if (GetGameState().GetEntities().size() < max_enemies_on_screen){
        auto pos = GetGameState().GetPlayers().front()->GetCenter();
        auto upper_left_bound = Vector2{pos.x - window_width / 2.0f, pos.y - window_height / 2.0f};


        auto slayer = std::make_unique<EnemySlayer>(&GetGameState(), upper_left_bound, window_width, window_height);
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
                exp+= GetRandomValue(2,5);
                if (exp >= 100){ exp_pts += 1; exp -= 100; }
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
            Scene::SetScene(std::make_unique<MenuScene>(window_width, window_height));
        }
    }
}

void SceneContinuousSpawn::draw(ImGui::Context& ctx) {
    GetRenderer().draw(ctx, GetGameState());

    DrawText(std::to_string(score).c_str(), window_width/2, 14, 64, {0,0,0,128});
    DrawText(std::to_string(score).c_str(), window_width/2, 10, 64, WHITE);

    DrawText((std::to_string(exp)+"/100 | "+std::to_string(exp_pts) + " points").c_str(), 16, window_height-64, 16, BLACK);
}
