#include "entity/enemy_slayer.hpp"
#include "entity/player.hpp"
#include "imgui/imgui.hpp"
#include "raylib-cpp.hpp"
#include "scene/scene.hpp"
#include <cstdint>
#include <memory>
#include "imgui/fpscounter.hpp"

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
	raylib::Window window(0, 0, "bounty of two", FLAG_FULLSCREEN_MODE | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	window.SetTargetFPS(120);

	Scene scene = Scene();
	auto player = std::make_unique<Player>(window.GetWidth()/2.0, window.GetHeight()/2.0);
	scene.add_entity(player.get());

	EnemySlayer slayer = EnemySlayer(window.GetWidth(),  window.GetHeight());
	slayer.SetTarget(player.get());
	scene.add_entity(&slayer);


	ImGui::Context ctx(true, player.get(), &scene, &window);

    scene.add_menu(new FPSCounter());


	while (!window.ShouldClose())
	{
		draw(window, ctx, scene);
		scene.update(window.GetFrameTime());
	}
}
