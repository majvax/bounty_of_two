#include "menu.hpp"
#include "scene.hpp"
#include "../imgui/menu.hpp"
#include "scene_continuous_spawn.hpp"

MenuScene::MenuScene(int width, int height) : Scene(width, height), title_texture("assets/title.png") {
    title_texture.SetFilter(TEXTURE_FILTER_BILINEAR);
}

void MenuScene::update(float deltatime, ImGui::Context& ctx) {
     if (raylib::Keyboard::IsKeyPressed(KEY_SPACE)) {
        auto new_scene = std::make_shared<SceneContinuousSpawn>(
            GetRenderer().GetWidth(), 
            GetRenderer().GetHeight(), 
            3
        );
        
        Scene::SetScene(new_scene);

        // Add player to the game state
        new_scene->GetGameState().add_player(
            std::make_unique<Player>(
                &new_scene->GetGameState(), 
                GetRenderer().GetWidth()/2.0, 
                GetRenderer().GetHeight()/2.0
            )
        );
        
        // Update the context with the player reference
        if (!new_scene->GetGameState().GetPlayers().empty()) {
            ctx.setPlayer(new_scene->GetGameState().GetPlayers()[0].get());
        }
    }
}

void MenuScene::draw(ImGui::Context& ctx) {
    title_texture.Draw(Vector2{GetRenderer().GetWidth() / 2.0f - title_texture.GetWidth() / 2.0f, GetRenderer().GetHeight() / 2.0f - title_texture.GetHeight() / 1.5f}, {255, 255, 255, 255});

    const char* text = "Press SPACE to play";
    int fontSize = 24;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, 
             GetRenderer().GetWidth() / 2.0f - textWidth / 2.0f,
             GetRenderer().GetHeight() / 2.0f + title_texture.GetHeight() / 1.5f + 20.0f, 
             fontSize, 
             BLACK);

}
