#include "esc_menu.hpp"
#include "menu.hpp"
#include "scene.hpp"
#include "../imgui/menu.hpp"
#include "scene_continuous_spawn.hpp"

EscMenuScene::EscMenuScene(int width, int height) : Scene(width, height), title_texture("assets/title.png") {
    title_texture.SetFilter(TEXTURE_FILTER_BILINEAR);
}

void EscMenuScene::update(float deltatime, ImGui::Context& ctx) {
    if (IsKeyPressed(KEY_SPACE))
    {
        Scene::PopScene();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (ctx.getWindow()) {
            ctx.getWindow()->Close();
        } else {
            std::exit(0);
        }
    }

    if (IsKeyPressed(KEY_M)) {
        auto menu_scene = std::make_shared<MenuScene>(
            ctx.getWindow()->GetWidth(), 
            ctx.getWindow()->GetHeight()
        );
        Scene::SetScene(menu_scene);
    }
}

void EscMenuScene::draw(ImGui::Context& ctx) {
    DrawRectangle(0, 0, GetRenderer().GetWidth(), GetRenderer().GetHeight(), {0, 0, 0, 150});
    
    while (ctx.lock())
    {
            ImVec2 center = ImVec2(ctx.getWindow()->GetWidth() * 0.5f, ctx.getWindow()->GetHeight() * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Appearing);
        
        if (ImGui::Begin("Escape Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
            ImGui::Spacing();
            
            // Title
            ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Game Paused").x) * 0.5f);
            ImGui::Text("Game Paused");
            ImGui::Separator();
            ImGui::Spacing();
              // Back to Game button
            ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - 250) * 0.5f);
            if (ImGui::Button("Back to Game", ImVec2(250, 35))) {
                Scene::PopScene();
            }
            
            ImGui::Spacing();
            
            // Back to Menu button
            ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - 250) * 0.5f);
            if (ImGui::Button("Back to Menu", ImVec2(250, 35))) {
                // Create and switch to main menu scene
                auto menu_scene = std::make_shared<MenuScene>(
                    ctx.getWindow()->GetWidth(), 
                    ctx.getWindow()->GetHeight()
                );
                Scene::SetScene(menu_scene);
            }
            
            ImGui::Spacing();
            
            // Quit to Desktop button
            ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - 250) * 0.5f);
            if (ImGui::Button("Quit to Desktop", ImVec2(250, 35))) {
                if (ctx.getWindow()) {
                    ctx.getWindow()->Close();
                } else {
                    std::exit(0);
                }
            }
              ImGui::Spacing();
        }
        ImGui::End();
    }
}
