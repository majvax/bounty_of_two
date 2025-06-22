#include "entity/enemy_slayer.hpp"
#include "entity/player.hpp"
#include "imgui/imgui.hpp"
#include <raylib-cpp.hpp>
#include "scene/scene_continuous_spawn.hpp"
#include "scene/game_state.hpp"
#include <cstdint>
#include <memory>
#include "imgui/fpscounter.hpp"
#include "imgui/player_gui.hpp"

void draw(raylib::Window& window, ImGui::Context& ctx, Scene& scene)
{
    while (window.Drawing())
    {
        window.ClearBackground(raylib::Color::White());
        scene.draw(ctx);
    }
}

int main()
{
    raylib::Window window(0, 0, "bounty of two", FLAG_FULLSCREEN_MODE | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);

    window.SetTargetFPS(0);
    HideCursor();

    SceneContinuousSpawn scene = SceneContinuousSpawn(window.GetWidth(), window.GetHeight(), 3);


    scene.GetGameState().add_player(std::make_unique<Player>(&scene.GetGameState(), window.GetWidth()/2.0, window.GetHeight()/2.0));

    ImGui::Context ctx(true);
    ctx.setWindow(&window);

    while (!window.ShouldClose())
    {
        scene.update(window.GetFrameTime(), ctx);
        draw(window, ctx, scene);
    }
}
