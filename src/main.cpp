#include "entity/enemy_slayer.hpp"
#include "entity/player.hpp"
#include "imgui/imgui.hpp"
#include "raylib-cpp.hpp"
#include "scene/scene.hpp"
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
	raylib::Window window(0, 0, "bounty of two", FLAG_FULLSCREEN_MODE | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);

	window.SetTargetFPS(120);

	Scene scene = Scene(window.GetWidth(), window.GetHeight());
	scene.add_entity(std::make_unique<Player>(window.GetWidth()/2.0, window.GetHeight()/2.0));

	auto slayer = std::make_unique<EnemySlayer>(window.GetWidth(),  window.GetHeight());
	slayer->SetTarget(scene.get_player());
    // move the slayer ownership to the scene
	scene.add_entity(std::move(slayer));


	ImGui::Context ctx(true, scene.get_player(), &scene, &window);

    // Ugly af, the menu should definitlely be added in the scene constructor
    scene.add_menu(std::make_unique<FPSCounter>());
    scene.add_menu(std::make_unique<PlayerMenu>());


	while (!window.ShouldClose())
	{
		scene.update(window.GetFrameTime());
		draw(window, ctx, scene);
	}
}
