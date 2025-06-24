#include "entity/enemy_slayer.hpp"
#include "entity/player.hpp"
#include "imgui/imgui.hpp"
#include <raylib-cpp.hpp>
#include "scene/scene_continuous_spawn.hpp"
#include "scene/game_state.hpp"
#include <cstdint>
#include <memory>

#include "scene/menu.hpp"
#include "sound/sound_manager.hpp"


void draw(raylib::Window& window, ImGui::Context& ctx, Scene* scene)
{
    while (window.Drawing() && scene != nullptr)
    {
        scene->draw(ctx);
    }
}

int main()
{
    raylib::Window window(0, 0, "bounty of two", FLAG_FULLSCREEN_MODE | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);

    window.SetTargetFPS(0);
    SetExitKey(0);
    DisableCursor();
    HideCursor();

    SoundManager::GetInstance()->play("assets/game.mp3");

;
    

    Scene::SetScene(std::make_shared<MenuScene>(window.GetWidth(), window.GetHeight()));



    ImGui::Context ctx(true);
    ctx.setWindow(&window);

    while (!window.ShouldClose())
    {
        SoundManager::GetInstance()->update();

        auto current_scenes = Scene::GetCurrentScenes();

        for (const auto& scene : current_scenes)
        {
            if (!scene) continue;
            scene->update(GetFrameTime(), ctx);
        }

        window.BeginDrawing();
        window.ClearBackground(raylib::Color::White());

        for (const auto& scene : current_scenes)
        {
            if (!scene) continue;
            scene->draw(ctx);
        }

        window.EndDrawing();
    }
}
