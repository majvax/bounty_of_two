#include "entity/enemy_slayer.hpp"
#include "entity/player.hpp"
#include "imgui/imgui.hpp"
#include <raylib-cpp.hpp>
#include "scene/scene_continuous_spawn.hpp"
#include "scene/game_state.hpp"
#include <cstdint>
#include <memory>

#include "scene/menu.hpp"



void draw(raylib::Window& window, ImGui::Context& ctx, Scene* scene)
{
    while (window.Drawing() && scene != nullptr)
    {
        window.ClearBackground(raylib::Color::White());
        scene->draw(ctx);
    }
}

int main()
{
    raylib::Window window(0, 0, "bounty of two", FLAG_FULLSCREEN_MODE | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);

    window.SetTargetFPS(0);
    HideCursor();


    Scene::SetCurrentScene(std::make_unique<MenuScene>(window.GetWidth(), window.GetHeight()));



    ImGui::Context ctx(true);
    ctx.setWindow(&window);

    while (!window.ShouldClose())
    {
        if (Scene::GetCurrentScene() == nullptr) {
            // If no scene is set, we can just continue to the next frame
            continue;
        }
        Scene::GetCurrentScene()->update(GetFrameTime(), ctx);
        draw(window, ctx, Scene::GetCurrentScene());
    }
}
