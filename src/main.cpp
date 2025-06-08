#include "entity/enemy_slayer.hpp"
#include "entity/player.hpp"
#include "imgui.hpp"
#include "../include/raylib/raylib-cpp.hpp"
#include "scene/scene.hpp"
#include <cstdint>

constexpr uint32_t NOEDIT_FLAGS = ImGuiWindowFlags_NoCollapse
| ImGuiWindowFlags_NoSavedSettings
| ImGuiWindowFlags_NoMove
| ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoTitleBar;

constexpr uint16_t WINDOW_WIDTH = 800;
constexpr uint16_t WINDOW_HEIGHT = 600;


void draw_imgui(raylib::Window& window, ImGui::Context& ctx)
{
	while (ctx.lock())
	{
		ImGui::SetNextWindowSize(ImVec2(100, 35), ImGuiCond_Once);
		ImGui::Begin("FPS Counter", nullptr, NOEDIT_FLAGS);
		ImGui::Text("FPS: %d", window.GetFPS());
		ImGui::End();
	}
}


void draw(raylib::Window& window, ImGui::Context& ctx, Scene& scene)
{
	while (window.Drawing())
	{
		window.ClearBackground(raylib::Color::White());


		// constexpr Vector2 size{ 20, 20 };
		// static raylib::Color col = raylib::Color::Blue();
		
		// // Draw a square in the center of the screen
		// col.DrawRectangle(
		// 	{ window.GetWidth() / 2 - size.x / 2, window.GetHeight() / 2 - size.y / 2 },
		// 	size
		// );

		scene.draw();


		// draw the imgui window on top of everything
		draw_imgui(window, ctx);
	}
}



int main()
{
	raylib::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "bounty of two");
	window.SetTargetFPS(120);
	ImGui::Context ctx(true);

	Scene scene = Scene();
	Player player = Player(WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0); // juste un test, le prof va nous tuer sinon mdr
	scene.add_entity(&player);

	EnemySlayer slayer = EnemySlayer(WINDOW_WIDTH, WINDOW_HEIGHT);
	slayer.SetTarget(&player);
	scene.add_entity(&slayer);

	while (!window.ShouldClose())
	{
		draw(window, ctx, scene);
		scene.update(window.GetFrameTime());
	}
}
